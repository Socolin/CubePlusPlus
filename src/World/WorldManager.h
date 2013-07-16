#ifndef WORLDMANAGER_H_
#define WORLDMANAGER_H_

#include "Util/Singleton.h"

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

class WorldManager : public Util::Singleton<WorldManager>
{
    friend Util::Singleton<WorldManager>;
    WorldManager();
public:
    virtual ~WorldManager();

    void Init();
    void Stop();
    bool IsRunning();

    void UpdateTick() const;

    EntityPlayer* LoadAndJoinWorld(const std::wstring& name, Network::NetworkSession* session);
    void RemovePlayer(EntityPlayer* player);

    void SendToAllPlayer(const Network::NetworkPacket& packet) const;
    void HandleChatMessage(EntityPlayer* player, std::wstring& message);

    World* GetWorld() const;

    bool IsFull() const;
    int GetPlayerCount() const;
    const std::string& GetName() const;
    const std::string& GetMotd() const;
    int GetMaxPlayerCount() const;
    void SetMaxPlayerCount(int maxPlayerCount);
private:
    Chat::ChatManager chatManager;
    std::set<EntityPlayer*> playerList;
    static WorldManager* instance;
    World* world;
    bool isRunning;
    int playerCount;
    int maxPlayerCount;
    std::string serverName;
    std::string serverMotd;
};

} /* namespace World */
#endif /* WORLDMANAGER_H_ */
