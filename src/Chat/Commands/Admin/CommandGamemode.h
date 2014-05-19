#ifndef COMMANDGAMEMODE_H_
#define COMMANDGAMEMODE_H_

#include "Chat/Commands/ChatCommand.h"

namespace Chat
{

class CommandGamemode : public ChatCommand
{
    enum eSyntax
    {
        SYNTAX_PLAYER_GAMEMODE,
        SYNTAX_GAMEMODE,
        SYNTAX_ERROR
    };
public:
    CommandGamemode(const CommandSender& sender, const std::vector<std::string>& splitedCommand);
    virtual bool CheckSyntax() override;
    virtual void ExecuteCommand() override;
private:
    eSyntax syntax;
};

} /* namespace Chat */
#endif /* COMMANDGAMEMODE_H_ */
