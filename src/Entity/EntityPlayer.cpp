#include "EntityPlayer.h"

#include <algorithm>

#include "Network/NetworkSession.h"
#include "Network/NetworkPacket.h"
#include "Network/Opcode.h"
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
    chunkToSend.push_back(std::make_pair(x, z));
}

void EntityPlayer::UpdateTick()
{
    if (session != NULL)
    {
        session->UpdateTick();
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

    Network::NetworkPacket packetAbilities(Network::OP_PLAYER_ABILITIES);
    unsigned char abilityFlag = 0;
    abilityFlag |= 0x1; // Damage disable
    abilityFlag |= 0x2; // Flying
    abilityFlag |= 0x4; // Can Fly
    abilityFlag |= 0x8; // Creative mode
    unsigned char walkingSpeed = 12;
    unsigned char flyingSpeed = 25;
    packetAbilities << abilityFlag << walkingSpeed << flyingSpeed;
    session->SendPacket(packetAbilities);

    Network::NetworkPacket packetUpdateTime(Network::OP_TIME_UPDATE);
    packetUpdateTime << (long) 0 << (long) 0;
    session->SendPacket(packetUpdateTime);

    Network::NetworkPacket packetPositionAndLook(Network::OP_PLAYER_POSITION_AND_LOOK);
    packetPositionAndLook << x << y << (y + 1.62) << z << (float) 0.f << (float) 0.f << false;
    session->SendPacket(packetPositionAndLook);

    Network::NetworkPacket packetUpdateHealth(Network::OP_UPDATE_HEALTH);
    short health = 20;
    short food = 20;
    float foodSaturation = 5.f;
    packetUpdateHealth << health << food << foodSaturation;
    session->SendPacket(packetUpdateHealth);

    Network::NetworkPacket packetUpdateExperience(Network::OP_SET_EXPERIENCE);
    float experienceBar = 0.5f;
    short level = 5;
    short totalXP = 130;
    packetUpdateExperience << experienceBar << level << totalXP;
    session->SendPacket(packetUpdateExperience);

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

} /* namespace World */
