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
    const std::string& GetDescription() const;
    int GetMaxPlayerCount() const;
    void SetMaxPlayerCount(int maxPlayerCount);
    bool IsOnlineMode() const;
    int GetLateness() const;
    void SetLateness(int lateness);
    void Kick(const std::wstring& playerName);
    void Ban(const std::wstring& playerName);
    void UnBan(const std::wstring& playerName);
    void SetAdmin(const std::wstring& playerName);
    void UnAdmin(const std::wstring& playerName);
    bool IsBan(const std::wstring& playerName);
    void Reload();
    bool IsAdmin(const std::wstring& playerName);

    EntityPlayer* GetPlayerByName(const std::wstring& playerName);
    std::map<std::wstring, EntityPlayer*>* GetPlayerByNameList();

private:
    void loadBanList();
    void loadAdminList();
    void loadMotd();
    Chat::ChatManager chatManager;
    std::set<EntityPlayer*> playerList;
    std::map<std::wstring, EntityPlayer*> playerByNameList;
    std::set<std::wstring> adminList;
    std::set<std::wstring> banList;
    static WorldManager* instance;
    World* world;
    bool isRunning;
    int playerCount;
    int maxPlayerCount;
    std::string serverName;
    std::string serverDescription;
    std::string* serverMotd;
    std::string banFileName;
    std::string adminFileName;
    int motdArraySize;
    bool onlineMode;
    unsigned int difficulty;
    int lateness;
};

} /* namespace World */
#endif /* WORLDMANAGER_H_ */
