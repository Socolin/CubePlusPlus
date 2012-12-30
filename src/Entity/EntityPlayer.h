#ifndef ENTITYPLAYER_H_
#define ENTITYPLAYER_H_

#include "LivingEntity.h"

#include <string>
#include <vector>

namespace Network
{
class NetworkSession;
}

namespace World
{
#define DEFAULT_WALKING_SPEED 12
#define DEFAULT_FLYING_SPEED 25
class EntityPlayer: public LivingEntity
{
    enum PlayerAbilities
    {
        DAMAGE_DISABLE = 0X1,
        FLYING = 0X2,
        CAN_FLY = 0X4,
        CREATIVE_MODE = 0X8
    };
public:
    EntityPlayer(double x, double y, double z, const std::wstring& name, Network::NetworkSession* session);
    virtual ~EntityPlayer();

    void AddChunkToSend(int x, int z);
    virtual void UpdateTick();
    void Respawn(double x, double y, double z);
    void JoinWorld();
    void Send(const Network::NetworkPacket& packet) const;
    virtual void GetCreatePacket(Network::NetworkPacket& packet);
    virtual void moveToVirtualChunk(int newVirtualChunkX, int newVirtualChunkZ);

    void Kick();
private:
    std::wstring name;
    std::vector<std::pair<int, int> > chunkToSend;
    Network::NetworkSession* session;
};

} /* namespace World */
#endif /* ENTITYPLAYER_H_ */
