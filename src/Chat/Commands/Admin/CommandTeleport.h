#ifndef COMMANDTELEPORT_H_
#define COMMANDTELEPORT_H_

#include "Chat/Commands/ChatCommand.h"

namespace Chat
{

class CommandTeleport: public ChatCommand
{
public:
    virtual bool CheckSyntax(const std::vector<std::string>& splitedCommand) const override;
    virtual void ExecuteCommand(const CommandSender& sender, std::vector<std::string> splitedCommand) const override;
};

} /* namespace Chat */
#endif /* COMMANDTELEPORT_H_ */
