#ifndef COMMANDTELEPORT_H_
#define COMMANDTELEPORT_H_

#include "Chat/Commands/ChatCommand.h"

namespace Chat
{

class CommandTeleport: public ChatCommand
{
    enum eSyntax
    {
        SYNTAX_PLR,
        SYNTAX_PLR_PLR,
        SYNTAX_X_Y_Z,
        SYNTAX_X_Y_Z_YAW_PITCH,
        SYNTAX_ERROR,
    };
public:
    CommandTeleport(const CommandSender& sender, const std::vector<std::string>& splitedCommand);
    virtual bool CheckSyntax() override;
    virtual void ExecuteCommand() override;
private:
    eSyntax syntax;
};

} /* namespace Chat */
#endif /* COMMANDTELEPORT_H_ */
