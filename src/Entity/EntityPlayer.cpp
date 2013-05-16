#include "EntityPlayer.h"

#include <algorithm>

#include "Block/Block.h"
#include "Block/BlockConstants.h"
#include "Block/BlockList.h"
#include "Entity/Object/EntityItem.h"
#include "Inventory/Item.h"
#include "Inventory/ItemStack.h"
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

EntityPlayer::EntityPlayer(double x, double y, double z, const std::wstring& name, Network::NetworkSession* session)
    : LivingEntity(ENTITY_TYPE_PLAYER, x, y, z)
    , gameMode(GAMEMODE_CREATVE)
    , name(name)
    , session(session)
    , currentWindowId(0)
    , animationId(-1)
    , currentWindow(nullptr)
{
    inventory = new Inventory::InventoryPlayer();
    clickedItem = new Inventory::Inventory(1);
    armorInventory = new Inventory::Inventory(4);//TODO: special inventory
    craftingInventory = new Inventory::Inventory(5);//TODO: special inventory

    inventoryWindow = new Window::Window(0, Window::WindowList::getWindowData(0));
    inventoryWindow->AddInventory(this, craftingInventory, 0);
    inventoryWindow->AddInventory(this, armorInventory, 5);
    inventoryWindow->AddInventory(this, inventory, 9);
}

EntityPlayer::~EntityPlayer()
{
    delete inventoryWindow;
    craftingInventory->CloseInventoryForDelete();
    armorInventory->CloseInventoryForDelete();
    clickedItem->CloseInventoryForDelete();
    inventory->CloseInventoryForDelete();

    delete craftingInventory;
    delete armorInventory;
    delete clickedItem;
    delete inventory;
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
    chunkToSend.push(std::pair<int, int>(x, z));
}

void EntityPlayer::UpdateTick()
{
    if (world != NULL)
    {
        if (session != NULL)
        {
            session->UpdateTick();
        }
        for (int i = 0; i < 15; i++)
        {
            if (!chunkToSend.empty())
            {
                // TODO: add check distance world->viewdistance
                world->RequestChunk(this, chunkToSend.front());
                chunkToSend.pop();
            }
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

    inventoryWindow->OpenWindow(this, false);
}

void EntityPlayer::GetCreatePacket(Network::NetworkPacket& packet)
{
    packet << (unsigned char) Network::OP_SPAWN_NAMED_ENTITY << entityId << name << networkX << networkY << networkZ << (char)  (yaw * 256.f / 360.f) << (char)  (pitch * 256.f / 360.f) << (unsigned short) 0 /* Current item*/;
    // Metadata
    packet << (char)0 << (char)0 << (unsigned char)127; // TODO: classe metadata
    packet << (unsigned char) Network::OP_ENTITY_HEAD_LOOK << entityId << ((char) (yaw * 256.f / 360.f));

    const Inventory::ItemStack* itemInHand = inventory->LookSlot(inventory->getHandSlotId());
    if (itemInHand != nullptr)
        packet << (unsigned char) Network::OP_ENTITY_EQUIPEMENT << entityId << (short)0 << itemInHand;
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

    VirtualSmallChunk*oldVChunk = world->GetVirtualSmallChunk(chunkX, chunkZ);
    oldVChunk->RemovePlayer(this);
    VirtualSmallChunk *vChunk = world->GetVirtualSmallChunk(newChunkX, newChunkZ);
    vChunk->AddPlayer(this);
    int viewDistance = world->getViewDistance();
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

void EntityPlayer::Kick()
{
    // TODO packet kick
    if (session != NULL)
        session->disconnect("kick");
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
        EntityItem* item = new EntityItem(this->x, this->y + getEyeHeight() - 0.3, this->z, itemToDrop, motionX, motionY, motionZ);
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
        DropItem(inventory->TakeSomeItemInSlot(inventory->getHandSlotId(), 1));
    }
}
void EntityPlayer::PlaceBlock(int x, unsigned char y, int z, char face, char cursorPositionX, char cursorPositionY, char cursorPositionZ)
{
    if (!world)
        return;

    // TODO check 6 block in range
    int clickedBlockId = world->GetBlockId(x, y, z);

    const Block::Block* block = Block::BlockList::Instance().getBlock(clickedBlockId);

    const Inventory::ItemStack* itemstack = inventory->LookSlot(inventory->getHandSlotId());
    const Inventory::Item* item = nullptr;
    if (itemstack != nullptr)
    {
        item = itemstack->getItem();
    }
    if (face != FACE_NONE)
    {
        if (block)
        {
            if (block->UseBlock(this, x, y, z, face, cursorPositionX, cursorPositionY, cursorPositionZ))
                return;
        }
        if (item != nullptr)
        {
            if (item->UseOnBlock(this, x, y, z, face, cursorPositionX, cursorPositionY, cursorPositionZ))
            {

            }
            else
            {
                ResetBlock(x, y, z);
            }
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
        packet << (unsigned char) Network::OP_ENTITY_EQUIPEMENT << entityId << (short)0 << inventory->LookSlot(inventory->getHandSlotId());
    }
    if (animationId >= 0)
    {
        packet << (unsigned char) Network::OP_ANIMATION << entityId << animationId;
        animationId = -1;
    }
}

void EntityPlayer::ResetBlock(int x, unsigned char y, int z)
{
    Network::NetworkPacket packet(Network::OP_MULTI_BLOCK_CHANGE);
    packet << (x >> 4) << (z >> 4) << (short)1 << (int)4;
    int blockData = world->GetBlockData(x, y, z);
    int blockID = world->GetBlockId(x, y, z);
    unsigned int dataChange = 0;
    dataChange |= blockData             & 0x0000000f;
    dataChange |= (blockID << 4)        & 0x0000fff0;
    dataChange |= ((int)y << 16)        & 0x00ff0000;
    dataChange |= ((z & 0xf) << 24)     & 0x0f000000;
    dataChange |= ((x & 0xf) << 28)     & 0xf0000000;
    packet << dataChange;
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
        currentWindow->CloseWindow(this, sendPacket);
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
        result = currentWindow->ClickOnWindow(this, slotId, button, action, mode, slot);
    }
    else if (currentWindow == nullptr)
    {
        result = inventoryWindow->ClickOnWindow(this, slotId, button, action, mode, slot);
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

Inventory::InventoryPlayer* EntityPlayer::GetInventory() const
{
    return inventory;
}

void EntityPlayer::UpdateInventories()
{
    inventory->SendUpdateToAllViewer();
    clickedItem->SendUpdateToAllViewer();
}

Window::Window* EntityPlayer::GetInventoryWindow() const
{
    return inventoryWindow;
}

} /* namespace World */
