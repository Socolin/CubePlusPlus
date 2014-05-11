
#ifndef CHATMANAGER_H_
#define CHATMANAGER_H_

#include <string>
#include <set>

namespace World
{
class EntityPlayer;
}

namespace Chat
{

class ChatManager
{
public:
    ChatManager();
    virtual ~ChatManager();
    bool HandleChatMessage(World::EntityPlayer* player, std::wstring& message);
    bool AddForbiddenWord(const std::wstring& word);
    bool RemoveForbiddenWord(const std::wstring& word);
    bool IsForbiddenWord(const std::wstring& word);
    bool AddMutedPlayer(const std::wstring& playerName);
    bool RemoveMutedPlayer(const std::wstring& playerName);
    bool IsMutedPlayer(const std::wstring& playerName);
private:
    bool handleAdminCommand(World::EntityPlayer* player, std::wstring& message);
    void loadForbiddenWordsList();
    void loadMutedPlayersList();
    void displayMatchingPrefix(World::EntityPlayer* player, const std::wstring& prefix);
    std::string fwFileName;
    std::string mutedPlayersFileName;
    std::set<std::wstring> forbiddenWords;
    std::set<std::wstring> mutedPlayers;
};

} /* namespace Chat */
#endif /* CHATMANAGER_H_ */
