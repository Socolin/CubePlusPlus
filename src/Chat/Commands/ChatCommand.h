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
#define COLOR_SYSTEM_PARAM WHITE

class ChatStream;
enum eSenderType
{
    PLAYER          = 0x1,
    COMMAND_BLOCK   = 0x2,
    CONSOLE         = 0x4,
};
#define ALL_SENDER      PLAYER | COMMAND_BLOCK | CONSOLE
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
    ChatCommand(const CommandSender& sender, const std::vector<std::string>& splitedCommand);
    virtual ~ChatCommand();
    virtual bool CheckSyntax();
    virtual void BadSyntaxMessage() const;
    virtual void ExecuteCommand();
    virtual bool CanUse();
protected:
    virtual void ExecuteCommandPlayer(World::EntityPlayer* plr);
    virtual void ExecuteCommandConsole();
    // FIXME commandBlock
    bool checkSyntaxtWith(const std::string& pattern) const;
    World::World* getWorldFromSender() const;
protected:
    const CommandSender sender;
    std::vector<std::string> splitedCommand;
};

} /* namespace Database */
#endif /* CHATCOMMAND_H_ */
