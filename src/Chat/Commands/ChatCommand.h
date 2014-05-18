#ifndef CHATCOMMAND_H_
#define CHATCOMMAND_H_

#include <string>
#include <vector>

#include "Chat/ChatStream.h"

namespace World
{
class EntityPlayer;
class World;
}
namespace Chat
{

#define COLOR_OK CYAN
#define COLOR_OK_PARAM GRAY
#define COLOR_KO ORANGE
#define COLOR_KO_PARAM GRAY
#define COLOR_SYSTEM YELLOW

class ChatStream;
enum eSenderType
{
    PLAYER,
    COMMAND_BLOCK,
    CONSOLE,
};
struct CommandSender
{
    CommandSender(eSenderType type, ChatStream& chatStream)
        : type(type)
        , chatStream(chatStream)
    {
    }
    union {
        World::EntityPlayer* plr;
        // CommandBlock
    } senderPtr;
    eSenderType type;
    ChatStream& chatStream;
};
class ChatCommand
{
public:
    ChatCommand();
    virtual ~ChatCommand();
    virtual bool CheckSyntax(const std::vector<std::string>& splitedCommand) const;
    virtual void BadSyntaxMessage(const CommandSender& sender) const;
    virtual void ExecuteCommand(const CommandSender& sender, std::vector<std::string> splitedCommand) const;
protected:
    virtual void ExecuteCommandPlayer(World::EntityPlayer* plr, ChatStream& chatStream, std::vector<std::string> splitedCommand) const;
    virtual void ExecuteCommandConsole(ChatStream& chatStream, std::vector<std::string> splitedCommand) const;
    // FIXME commandBlock
    bool checkSyntaxtWith(const std::string& pattern,  std::vector<std::string> splitedCommand) const;
    World::World* getWorldFromSender(const CommandSender& sender) const;
};

} /* namespace Database */
#endif /* CHATCOMMAND_H_ */
