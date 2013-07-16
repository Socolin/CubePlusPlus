#ifndef WORLDMANAGER_H_
#define WORLDMANAGER_H_

#include "Util/Singleton.h"

#include <string>
#include <set>
#include <map>

#include "Chat/ChatManager.h"

namespace Network
{
class NetworkSession;
class NetworkPacket;
}

namespace World
{
enum eDifficulty
{
    PEACEFUL,
    EASY,
    NORMAL,
    HARD,
    DIFFICULTY_MAX
};

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
    bool IsOnlineMode() const;
private:
    Chat::ChatManager chatManager;
    std::set<EntityPlayer*> playerList;
    std::map<std::wstring, EntityPlayer*> playerByNameList;
    static WorldManager* instance;
    World* world;
    bool isRunning;
    int playerCount;
    int maxPlayerCount;
    std::string serverName;
    std::string serverMotd;
    bool onlineMode;
    unsigned int difficulty;
};

} /* namespace World */
#endif /* WORLDMANAGER_H_ */
