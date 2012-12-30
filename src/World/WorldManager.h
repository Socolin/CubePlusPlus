#ifndef WORLDMANAGER_H_
#define WORLDMANAGER_H_

#include <string>
#include <set>
namespace Network
{
class NetworkSession;
}

namespace World
{

class EntityPlayer;
class World;

class WorldManager
{
public:
    static WorldManager* GetInstance();
    virtual ~WorldManager();

    void Init();
    void UpdateTick() const;

    EntityPlayer* LoadAndJoinWorld(const std::wstring& name, Network::NetworkSession* session);
    void RemovePlayer(EntityPlayer* player);

    void Stop();
    bool IsRunning();
private:
    WorldManager();
    std::set<EntityPlayer*> playerList;
    static WorldManager* instance;
    World* world;
    bool isRunning;
};

} /* namespace World */
#endif /* WORLDMANAGER_H_ */
