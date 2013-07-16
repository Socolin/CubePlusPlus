#include "EntityPlayer.h"

#include <algorithm>

#include "Block/Block.h"
#include "Block/BlockConstants.h"
#include "Block/BlockList.h"
#include "Entity/Object/EntityItem.h"
#include "Inventory/Item.h"
#include "Inventory/ItemStack.h"
#include "Inventory/InventoryCraft.h"
#include "Network/NetworkSession.h"
#include "Network/NetworkPacket.h"
#include "Network/OpcodeList.h"
#include "Util/FloatUtil.h"
#include "Window/Window.h"
#include "Window/WindowList.h"
#include "Window/WindowStaticData.h"
#include "World/VirtualChunk.h"
#include "World/VirtualSmallChunk.h"
#include "World/World.h"

namespace World
{

EntityPlayer::EntityPlayer(const Position& spawnPosition, const std::wstring& name, Network::NetworkSession* session)
    : LivingEntity(ENTITY_TYPE_PLAYER, 0, spawnPosition.x, spawnPosition.y, spawnPosition.z)
    , gameMode(GAMEMODE_CREATVE)
    , name(name)
    , session(session)
    , currentWindowId(0)
    , animationId(-1)
    , currentWindow(nullptr)
{
    mainInventory = new Inventory::Inventory(27, Inventory::INVENTORY_TYPE_PLAYER_MAIN);
    handsInventory = new Inventory::InventoryPlayer();
    enderChestInventory = new Inventory::Inventory(27);
    clickedItem = new Inventory::Inventory(1);
    armorInventory = new Inventory::Inventory(4);//TODO: special inventory
    craftingInventory = new Inventory::InventoryCraft(2, 2);

    inventoryWindow = new Window::Window(0, this, Window::WindowList::getWindowData(0));
    inventoryWindow->AddInventory(craftingInventory);
    inventoryWindow->AddInventory(armorInventory);
    inventoryWindow->AddInventory(mainInventory);
    inventoryWindow->AddInventory(handsInventory, Window::Window::PRIORITY_HIGH);
}

EntityPlayer::~EntityPlayer()
{
    delete inventoryWindow;
    craftingInventory->CloseInventoryForDelete();
    armorInventory->CloseInventoryForDelete();
    clickedItem->CloseInventoryForDelete();
    handsInventory->CloseInventoryForDelete();
    mainInventory->CloseInventoryForDelete();
    enderChestInventory->CloseInventoryForDelete();

    delete craftingInventory;
    delete armorInventory;
    delete clickedItem;
    delete handsInventory;
    delete mainInventory;
    delete enderChestInventory;
}

void EntityPlayer::Send(const Network::NetworkPacket& packet) const
{
    if (session != NULL)
    {
        session->SendPacket(packet);
    }
}
void EntityPlayer::AddChunkToSend(int x, int z)
{
    chunkToSend.push({x, z, abs(chunkX - x) + abs(chunkZ - z)});
}

void EntityPlayer::UpdateTick()
{
    if (session == nullptr)
        return;
    if (world != nullptr)
    {
        for (int i = 0; i < 15; i++)
        {
            if (!chunkToSend.empty())
            {
                if (session == nullptr)
                    return;
                if (session->HasPendingData())
                    break;
                const ChunkToSendData& chunkToSendData =  chunkToSend.top();
                world->RequestChunk(this, chunkToSendData.x, chunkToSendData.z);
                chunkToSend.pop();
            }
        }

        std::vector<Entity*> entityList;
        tempBoundingBox.SetAndExtend(boundingBox, 1, 0.5, 1);
        world->GetEntitiesInAABB(entityId, tempBoundingBox, entityList);
        for (Entity* entity : entityList)
        {
            entity->OnCollideWithPlayer(this);
        }
    }
    UpdateInventories();
}

void EntityPlayer::Respawn(double x, double y, double z)
{
    //TODO: override relocate to handle well movement etc...
    Relocate(x, y, z);
    Network::NetworkPacket packet;

    // Move this to session
    Network::NetworkPacket packetInitialPosition(Network::OP_PLAYER_POSITION_AND_LOOK);
    packetInitialPosition << x << y << z << (float) 0 << (float) 0 << (char) 0;
    session->SendPacket(packetInitialPosition);
}

void EntityPlayer::OnJoinWorld()
{
    // TODO: get it from world
    Network::NetworkPacket packetSpawnPosition(Network::OP_SPAWN_POSITION);
    packetSpawnPosition << (int) x << (int) y << (int) z;
    session->SendPacket(packetSpawnPosition);

    session->SendSetAbilities(DEFAULT_FLYING_SPEED, DEFAULT_WALKING_SPEED,  DAMAGE_DISABLE | FLYING | CAN_FLY | CREATIVE_MODE);

    // TODO: use world value
    session->SendUpdateTime(0, 0);

    session->SendSetPositionAndLook(x, y, y + 1.62, z, 0.f, 0.f, false);

    session->SendUpdateHealth(20,20,5.f);

    session->SendSetExperience(0, 0.f, 0);

    inventoryWindow->OpenWindow(false);
}

void EntityPlayer::GetCreatePacket(Network::NetworkPacket& packet)
{
    packet << (unsigned char) Network::OP_SPAWN_NAMED_ENTITY << entityId << name << networkX << networkY << networkZ << (char)  (yaw * 256.f / 360.f) << (char)  (pitch * 256.f / 360.f) << (unsigned short) 0 /* Current item*/;
    // Metadata
    packet << (char)0 << (char)0 << (unsigned char)127; // TODO: classe metadata
    packet << (unsigned char) Network::OP_ENTITY_HEAD_LOOK << entityId << ((char) (yaw * 256.f / 360.f));

    const Inventory::ItemStack* itemInHand = handsInventory->LookSlot(handsInventory->getHandSlotId());
    if (itemInHand != nullptr)
        packet << (unsigned char) Network::OP_ENTITY_EQUIPEMENT << entityId << (short)0 << itemInHand;

    packet << (unsigned char) Network::OP_ENTITY_METADATA
            << entityId;
    metadataManager.Write(packet);
}

void EntityPlayer::moveToVirtualChunk(int newVirtualChunkX, int newVirtualChunkZ)
{
    VirtualChunk *oldVChunk = world->GetVirtualChunk(virtualChunkX, virtualChunkZ);
    oldVChunk->RemovePlayerByMoving(this, newVirtualChunkX, newVirtualChunkZ);
    VirtualChunk *vChunk = world->GetVirtualChunk(newVirtualChunkX, newVirtualChunkZ);
    vChunk->AddPlayerByMoving(this, virtualChunkX, virtualChunkZ);
}
void EntityPlayer::moveToChunk(int newChunkX, int newChunkZ)
{
    int viewDistance = world->getViewDistance();

    while (!chunkToSend.empty())
    {
        const ChunkToSendData& chunkToSendData =  chunkToSend.top();
        if (abs(newChunkX - chunkToSendData.x) <= viewDistance && abs(newChunkZ - chunkToSendData.z) <= viewDistance)
        {
            sortChunkToSend.push_back({chunkToSendData.x, chunkToSendData.z, abs(newChunkX - chunkToSendData.x) + abs(newChunkZ - chunkToSendData.z)});
        }
        chunkToSend.pop();
    }

    for (const ChunkToSendData& chunkToSendData : sortChunkToSend)
    {
        chunkToSend.push(chunkToSendData);
    }
    sortChunkToSend.clear();

    VirtualSmallChunk*oldVChunk = world->GetVirtualSmallChunk(chunkX, chunkZ);
    oldVChunk->RemovePlayer(this);
    VirtualSmallChunk *vChunk = world->GetVirtualSmallChunk(newChunkX, newChunkZ);
    vChunk->AddPlayer(this);

    if (newChunkX != chunkX)
    {
        int offsetX = newChunkX - chunkX; // -1 or 1
        int xRemove = chunkX - viewDistance * offsetX;
        int xAdd = newChunkX + viewDistance * offsetX;
        for (int z = chunkZ - viewDistance; z <= chunkZ + viewDistance; z++)
        {
            Chunk* chunk = world->GetChunk(xAdd, z);
            chunk->AddPlayer(this);
            AddChunkToSend(xAdd, z);
            chunk = world->GetChunk(xRemove, z);
            chunk->RemovePlayer(this);
        }
    }
    if (newChunkZ != chunkZ) // Déplacement sur l'axe Z
    {
        int offsetZ = newChunkZ - chunkZ;// -1 or 1
        int zRemove = chunkZ - viewDistance * offsetZ;
        int zAdd = newChunkZ + viewDistance * offsetZ;
        for (int x = newChunkX - viewDistance; x <= newChunkX + viewDistance; x++)
        {
            Chunk* chunk = world->GetChunk(zAdd, x);
            chunk->AddPlayer(this);
            AddChunkToSend(x, zAdd);
            chunk = world->GetChunk(zRemove, x);
            chunk->RemovePlayer(this);
        }
    }
}

void EntityPlayer::Kick(const std::wstring message)
{
    if (session != NULL)
        session->kick(message);
    session = nullptr;
}

void EntityPlayer::Disconnect()
{
    session = nullptr;
}

EntityPlayer::eGameMode EntityPlayer::GetGameMode() const
{
    return gameMode;
}


const std::wstring& EntityPlayer::GetUsername()
{
    return name;
}

void EntityPlayer::DropItem(Inventory::ItemStack* itemToDrop)
{
    if (itemToDrop == nullptr)
        return;
    const Inventory::Item* item = itemToDrop->getItem();
    if (item != nullptr)
    {
        const double speed = 0.3F;
        double motionX = -sin(yaw/ 180.0F * M_PI) * cos(pitch / 180.0F * M_PI) * speed;
        double motionZ = cos(yaw / 180.0F * M_PI) * cos(pitch / 180.0F * M_PI) * speed;
        double motionY = -sin(pitch / 180.0F * M_PI) * speed + 0.1F;

        double modifier = 0.02F;
        float randomModifier =  Util::randFloat() * M_PI * 2.0F;
        modifier *= Util::randFloat();
        motionX += cos(randomModifier) * modifier;
        motionY += (Util::randFloat() - Util::randFloat()) * 0.1f;
        motionZ += sin(randomModifier) * modifier;
        EntityItem* item = new EntityItem(this->x, this->y + getEyeHeight() - 0.3, this->z, itemToDrop, motionX, motionY, motionZ, 40);
        world->AddEntity(item);
    }
}
void EntityPlayer::DigBlock(int state, int x, unsigned char y, int z, char /*face*/)
{
    if (!world)
        return;
    if (state == 0)
    {
        world->RemoveBlock(x, y, z);
    }
    else if (state == 4)
    {
        DropItem(handsInventory->TakeSomeItemInSlot(handsInventory->getHandSlotId(), 1));
    }
}
void EntityPlayer::PlaceBlock(int x, unsigned char y, int z, char face, char cursorPositionX, char cursorPositionY, char cursorPositionZ)
{
    if (!world)
        return;

    // TODO check 6 block in range
    int clickedBlockId = world->GetBlockId(x, y, z);

    const Block::Block* block = Block::BlockList::Instance().getBlock(clickedBlockId);

    const Inventory::ItemStack* itemstack = handsInventory->LookSlot(handsInventory->getHandSlotId());
    const Inventory::Item* item = nullptr;
    if (itemstack != nullptr)
    {
        item = itemstack->getItem();
    }
    if (face != FACE_NONE)
    {
        if (block && !IsSneak())
        {
            if (block->UseBlock(this, x, y, z, face, cursorPositionX, cursorPositionY, cursorPositionZ))
                return;
        }
        if (item != nullptr)
        {
            if (item->UseOnBlock(this, x, y, z, face, cursorPositionX, cursorPositionY, cursorPositionZ))
            {
                return;
            }
            else
            {
                ResetBlock(x, y, z);
            }
        }
        if (block && IsSneak())
        {
            if (block->UseBlock(this, x, y, z, face, cursorPositionX, cursorPositionY, cursorPositionZ))
                return;
        }
    }
    else
    {
        if (item != nullptr)
        {
            if (item->Use(this))
            {

            }
        }
    }
}

void EntityPlayer::GetSpecificUpdatePacket(Network::NetworkPacket& packet)
{
    if (hasChangeItemInHand)
    {
        hasChangeItemInHand = false;
        packet << (unsigned char) Network::OP_ENTITY_EQUIPEMENT << entityId << (short)0 << handsInventory->LookSlot(handsInventory->getHandSlotId());
    }
    if (animationId >= 0)
    {
        packet << (unsigned char) Network::OP_ANIMATION << entityId << animationId;
        animationId = -1;
    }

    if (metadataManager.HasChanged())// Move it to entity class
    {
        metadataManager.ClearChange();
        packet << (unsigned char) Network::OP_ENTITY_METADATA
                << entityId;
        metadataManager.Write(packet);
    }
}

void EntityPlayer::ResetBlock(int x, unsigned char y, int z)
{
    Network::NetworkPacket packet(Network::OP_BLOCK_CHANGE);
    i_data blockData = world->GetBlockData(x, y, z);
    i_block blockID = world->GetBlockId(x, y, z);
    packet << x << y << z << blockID << blockData;
    Send(packet);
}

void EntityPlayer::UseEntity(int target, bool leftClick)
{
    Entity* entity = world->GetEntityById(target);
    if (entity)
    {
        if (!leftClick)
            entity->Interact(this);
    }
}

void EntityPlayer::ItemInHandHasChange()
{
    hasChangeItemInHand = true;
}

void EntityPlayer::PlayAnimation(char animationId)
{
    this->animationId = animationId;
}

void EntityPlayer::OpenWindow(Window::Window* window)
{
    if (currentWindow != nullptr)
    {
        CloseWindow(currentWindow->GetId(), true);
    }
    currentWindow = window;
}

void EntityPlayer::CloseWindow(i_windowId windowId, bool sendPacket)
{
    if (windowId == currentWindowId && currentWindow != nullptr && currentWindow->GetId() == windowId)
    {
        currentWindow->CloseWindow(sendPacket);
        delete currentWindow;
        currentWindow = nullptr;
        inventoryWindow->ReOpenAllInventories(this);
    }
}

i_windowId EntityPlayer::GetNextAndSetCurrentWindowId()
{
    this->currentWindowId = (this->currentWindowId % 100) + 1;
    return this->currentWindowId;
}

void EntityPlayer::ClickOnWindow(i_windowId windowId, short slotId, char button, short action, char mode, const Inventory::ItemStack* slot)
{
    bool result = false;
    if (windowId == currentWindowId && currentWindow != nullptr && currentWindow->GetId() == windowId)
    {
        result = currentWindow->ClickOnWindow(slotId, button, action, mode, slot);
    }
    else if (currentWindow == nullptr)
    {
        result = inventoryWindow->ClickOnWindow(slotId, button, action, mode, slot);
    }
    Network::NetworkPacket confirmTransactionPacket(Network::OP_CONFIRM_TRANSACTION);
    confirmTransactionPacket << windowId << action << result;
    Send(confirmTransactionPacket);
}

Inventory::Inventory* EntityPlayer::GetClickedItem() const
{
    return clickedItem;
}

Inventory::Inventory* EntityPlayer::GetCraftingInventory() const
{
    return craftingInventory;
}

Inventory::Inventory* EntityPlayer::GetArmorInventory() const
{
    return armorInventory;
}

Inventory::Inventory* EntityPlayer::GetMainInventory() const
{
    return mainInventory;
}

Inventory::InventoryPlayer* EntityPlayer::GetHandsInventory() const
{
    return handsInventory;
}

Inventory::Inventory* EntityPlayer::GetEnderChestInventory() const
{
    return enderChestInventory;
}

void EntityPlayer::UpdateInventories()
{
    mainInventory->SendUpdateToAllViewer();
    handsInventory->SendUpdateToAllViewer();
    clickedItem->SendUpdateToAllViewer();
}

void EntityPlayer::DoAction(char action)
{
    switch (action)
    {
    case ACTION_CROUCH:
        SetSneak(true);
        break;
    case ACTION_UNCROUCH:
        SetSneak(false);
        break;
    case ACTION_LEAVE_BED:
        // TODO
        break;
    case ACTION_START_SPRINTING:
        SetSprinting(true);
        break;
    case ACTION_STOP_SPRINTING:
        SetSprinting(false);
        break;
    default:
        std::cerr << "Invalid action:" << action << std::endl;
        break;
    }
}

bool EntityPlayer::Load(nbt::TagCompound* tagNbtData)
{
    return parent_type::Load(tagNbtData);
}

bool EntityPlayer::Save(nbt::TagCompound* tagNbtData)
{
    return parent_type::Save(tagNbtData);
}

Window::Window* EntityPlayer::GetInventoryWindow() const
{
    return inventoryWindow;
}

} /* namespace World */
