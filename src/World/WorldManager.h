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

    /**
     * Initialize worlds
     */
    void Init();

    /**
     * Kick all player
     * Save world
     * Stop server
     */
    void Stop();

    /**
     * Check if server is running, return true until WorldManager::Stop is call
     * @return
     */
    bool IsRunning();

    /**
     * Update world
     */
    void UpdateTick() const;


    EntityPlayer* LoadAndJoinWorld(const std::wstring& name, Network::NetworkSession* session);
    void RemovePlayer(EntityPlayer* player);

    void SendToAllPlayer(const Network::NetworkPacket& packet) const;

    /**
     * Handle a chat message sended by player
     * @param player playet who send message
     * @param message message sended
     */
    void HandleChatMessage(EntityPlayer* player, std::wstring& message);

    World* GetWorld() const;

    /**
     * Check if server is full
     * @return true if there is MaxPlayer online
     */
    bool IsFull() const;

    /**
     * Get name of server, defined in config,
     * not used in minecraft code, can be used to manage multiple servers
     * @return
     */
    const std::string& GetName() const;

    /**
     * Get description of server (sended in ping)
     * @return
     */
    const std::string& GetDescription() const;

    /**
     * Return number of player online
     * @return
     */
    int GetPlayerCount() const;

    /**
     * Get number of avaible slot
     * @return
     */
    int GetMaxPlayerCount() const;

    /**
     * Change number of max player IG, if new max < oldMax, player will not be kicked
     * @param maxPlayerCount
     */
    void SetMaxPlayerCount(int maxPlayerCount);

    /**
     * Return true if server use authentification from mojang server or not.
     * @return
     */
    bool IsOnlineMode() const;

    /**
     * Reload admin ban and whitelist file
     */
    void Reload();


    int GetLateness() const;
    void SetLateness(int lateness);

    void Kick(const std::wstring& playerName);
    bool Ban(const std::wstring& playerName);
    bool UnBan(const std::wstring& playerName);
    bool SetAdmin(const std::wstring& playerName, EntityPlayer*& player);
    bool UnAdmin(const std::wstring& playerName, EntityPlayer*& player);
    bool AddToWhitelist(const std::wstring& playerName);
    bool UnWhitelist(const std::wstring& playerName);
    bool Mute(const std::wstring& playerName, EntityPlayer*& player);
    bool UnMute(const std::wstring& playerName, EntityPlayer*& player);
    bool IsBan(const std::wstring& playerName);
    bool IsAdmin(const std::wstring& playerName);
    bool IsWhitelisted(const std::wstring& playerName);
    bool IsMuted(const std::wstring& playerName);

    EntityPlayer* GetPlayerByName(const std::wstring& playerName);
    std::map<std::wstring, EntityPlayer*>* GetPlayerByNameList();

private:
    void loadBanList();
    void loadAdminList();
    void loadMotd();
    void loadWhitelist();
    void loadMutedPlayersList();
    Chat::ChatManager chatManager;
    std::set<EntityPlayer*> playerList;
    std::map<std::wstring, EntityPlayer*> playerByNameList;
    std::set<std::wstring> adminList;
    std::set<std::wstring> banList;
    std::set<std::wstring> whitelist;
    std::set<std::wstring> mutedPlayers;
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
    std::string whitelistFileName;
    std::string mutedPlayersFileName;
    bool useWhitelist;
    int motdArraySize;
    bool onlineMode;
    unsigned int difficulty;
    int lateness;
};

} /* namespace World */
#endif /* WORLDMANAGER_H_ */
