#include "EntityPlayer.h"

#include <algorithm>

#include "Network/NetworkSession.h"
#include "Network/NetworkPacket.h"
#include "Network/OpcodeList.h"
#include "World/VirtualChunk.h"
#include "World/World.h"

namespace World
{

EntityPlayer::EntityPlayer(double x, double y, double z, const std::wstring& name, Network::NetworkSession* session) :
        LivingEntity(x, y, z), name(name), session(session)
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

    session->SendSetAbilities(DEFAULT_WALKING_SPEED, DEFAULT_FLYING_SPEED,  DAMAGE_DISABLE | FLYING | CAN_FLY | CREATIVE_MODE);

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
}

void EntityPlayer::moveToVirtualChunk(int newVirtualChunkX, int newVirtualChunkZ)
{
    VirtualChunk *oldVChunk = world->GetVirtualChunk(virtualChunkX, virtualChunkZ);
    oldVChunk->RemovePlayerByMoving(this, newVirtualChunkX, newVirtualChunkZ);
    VirtualChunk *vChunk = world->GetVirtualChunk(newVirtualChunkX, newVirtualChunkZ);
    vChunk->AddPlayerByMoving(this, virtualChunkX, virtualChunkZ);
}

void EntityPlayer::Kick()
{
    // TODO packet kick
    if (session != NULL)
        session->disconnect("kick");
    session = NULL;
}

const std::wstring& EntityPlayer::GetUsername()
{
    return name;
}

} /* namespace World */
