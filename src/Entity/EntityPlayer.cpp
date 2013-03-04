#include "EntityPlayer.h"

#include <algorithm>

#include "Block/Block.h"
#include "Block/BlockConstants.h"
#include "Block/BlockList.h"
#include "Inventory/Item.h"
#include "Inventory/ItemStack.h"
#include "Network/NetworkSession.h"
#include "Network/NetworkPacket.h"
#include "Network/OpcodeList.h"
#include "World/VirtualChunk.h"
#include "World/World.h"

namespace World
{

EntityPlayer::EntityPlayer(double x, double y, double z, const std::wstring& name, Network::NetworkSession* session) :
    LivingEntity(x, y, z), name(name), session(session),animationId(-1)
{

}

EntityPlayer::~EntityPlayer()
{
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
        if (!chunkToSend.empty())
        {
            // TODO: add check distance world->viewdistance
            world->RequestChunk(this, chunkToSend.front());
            chunkToSend.pop();
        }
    }
}

void EntityPlayer::Respawn(double x, double y, double z)
{
    Relocate(x, y, z);
    Network::NetworkPacket packet;

    // Move this to session
    Network::NetworkPacket packetInitialPosition(Network::OP_PLAYER_POSITION_AND_LOOK);
    packetInitialPosition << x << y << z << (float) 0 << (float) 0 << (char) 0;
    session->SendPacket(packetInitialPosition);
}

void EntityPlayer::JoinWorld()
{
    Network::NetworkPacket packetRespawn(Network::OP_SPAWN_POSITION);
    packetRespawn << (int) 0 << (int) 100 << (int) 0;
    session->SendPacket(packetRespawn);

    session->SendSetAbilities(DEFAULT_FLYING_SPEED, DEFAULT_WALKING_SPEED,  DAMAGE_DISABLE | FLYING | CAN_FLY | CREATIVE_MODE);

    session->SendUpdateTime(0, 0);

    session->SendSetPositionAndLook(x, y, y + 1.62, z, 0.f, 0.f, false);

    session->SendUpdateHealth(20,20,5.f);

    session->SendSetExperience(0, 0.f, 0);

    Network::NetworkPacket packetSetInventory(Network::OP_SET_WINDOW_ITEMS);
    unsigned char windowId = 0; // 0 = player inventory
    short countItem = 0;
    packetSetInventory << windowId << countItem;
    session->SendPacket(packetSetInventory);
}

void EntityPlayer::Send(const Network::NetworkPacket& packet) const
{
    if (session != NULL)
    {
        session->SendPacket(packet);
    }
}

void EntityPlayer::GetCreatePacket(Network::NetworkPacket& packet)
{
    packet << (unsigned char) Network::OP_SPAWN_NAMED_ENTITY << entityId << name << networkX << networkY << networkZ << (char)  (yaw * 256.f / 360.f) << (char)  (pitch * 256.f / 360.f) << (unsigned short) 0 /* Current item*/;
    // Metadata
    packet << (char)0 << (char)0 << (unsigned char)127; // TODO: classe metadata
    packet << (unsigned char) Network::OP_ENTITY_HEAD_LOOK << entityId << ((char) (yaw * 256.f / 360.f));

    const Inventory::ItemStack& itemInHand = inventory.GetItemInHand();
    if (itemInHand.getItemId() != -1)
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
    int viewDistance = world->getViewDistance();
    if (newChunkX != chunkX)
    {
        int offsetX = newChunkX - chunkX;
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
        int offsetZ = newChunkZ - chunkZ;
        int zRemove = chunkZ - viewDistance * offsetZ;
        int zAdd = newChunkZ + viewDistance * offsetZ;
        for (int x = chunkX - viewDistance; x <= chunkX + viewDistance; x++)
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

const std::wstring& EntityPlayer::GetUsername()
{
    return name;
}

Inventory::InventoryPlayer& EntityPlayer::GetInventory()
{
    return inventory;
}
void EntityPlayer::DigBlock(int state, int x, unsigned char y, int z, char face)
{
    if (state == 0)
    {
        Chunk* chunk = world->GetChunk(x >> 4, z >> 4);
        chunk->ChangeBlock(x & 0xf, y, z & 0xf, 0, 0);
    }
}
void EntityPlayer::PlaceBlock(int x, unsigned char y, int z, char face, char CursorpositionX, char CursorpositionY, char CursorpositionZ)
{
    // TODO check 6 block in range
    int clickedBlockId = world->GetBlockId(x, y, z);
    Block::Block* block = Block::BlockList::Instance()->blocks[clickedBlockId];
    Inventory::ItemStack& itemstack = inventory.GetItemInHand();
    Inventory::Item* item = itemstack.getItem();
    if (face != FACE_NONE)
    {
        if (block)
        {
            if (block->UseBlock(this, x, y, z, face, itemstack, CursorpositionX, CursorpositionY, CursorpositionZ))
                return;
        }
        if (item != nullptr)
            item->UseOnBlock(this, x, y, z, face, itemstack, CursorpositionX, CursorpositionY, CursorpositionZ);
    }
}

void EntityPlayer::GetSpecificUpdatePacket(Network::NetworkPacket& packet)
{
    if (hasChangeItemInHand)
    {
        hasChangeItemInHand = false;
        packet << (unsigned char) Network::OP_ENTITY_EQUIPEMENT << entityId << (short)0 << inventory.GetItemInHand();
    }
    if (animationId >= 0)
    {
        packet << (unsigned char) Network::OP_ANIMATION << entityId << animationId;
        animationId = -1;
    }
}

void EntityPlayer::PlayAnimation(char animationId)
{
    this->animationId = animationId;
}

} /* namespace World */
