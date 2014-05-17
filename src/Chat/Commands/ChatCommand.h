#ifndef CHATCOMMAND_H_
#define CHATCOMMAND_H_

#include <string>
#include <vector>

namespace World
{
class EntityPlayer;
}
namespace Chat
{
class ChatStream;
enum eSenderType
{
    PLAYER,
    COMMAND_BLOCK,
    CONSOLE,
};
struct CommandSender
{
    union {
        World::EntityPlayer* plr;
        // CommandBlock
    } senderPtr;
    eSenderType type;
    ChatStream* chatStream;
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
    virtual void ExecuteCommandPlayer(World::EntityPlayer* plr, ChatStream* chatStream, std::vector<std::string> splitedCommand) const;
    virtual void ExecuteCommandConsole(ChatStream* chatStream, std::vector<std::string> splitedCommand) const;
    // FIXME commandBlock

    bool checkSyntaxtWith(const std::string& pattern,  std::vector<std::string> splitedCommand) const;
};

} /* namespace Database */
#endif /* CHATCOMMAND_H_ */
