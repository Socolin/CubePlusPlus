#ifndef WORLDMANAGER_H_
#define WORLDMANAGER_H_

#include <string>
#include <set>

#include "Chat/ChatManager.h"

namespace Network
{
class NetworkSession;
class NetworkPacket;
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

    void HandleChatMessage(EntityPlayer* player, std::wstring& message);

    int getPlayerCount();
    void Stop();
    bool IsRunning();
    void SendToAllPlayer(Network::NetworkPacket& packet);

    World* GetWorld();
private:
    Chat::ChatManager chatManager;
    WorldManager();
    std::set<EntityPlayer*> playerList;
    static WorldManager* instance;
    World* world;
    bool isRunning;
};

} /* namespace World */
#endif /* WORLDMANAGER_H_ */
