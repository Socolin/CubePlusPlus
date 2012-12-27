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

class EntityPlayer: public LivingEntity
{
public:
    EntityPlayer(double x, double y, double z, const std::wstring& name, Network::NetworkSession* session);
    virtual ~EntityPlayer();

    void AddChunkToSend(int x, int z);
    virtual void UpdateTick();
    void Respawn(double x, double y, double z);
    void JoinWorld();
    void Send(const Network::NetworkPacket& packet) const;
    virtual void GetCreatePacket(Network::NetworkPacket& packet);
private:
    std::wstring name;
    std::vector<std::pair<int, int> > chunkToSend;
    Network::NetworkSession* session;
};

} /* namespace World */
#endif /* ENTITYPLAYER_H_ */
