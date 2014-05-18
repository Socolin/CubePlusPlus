#ifndef COMMANDBAN_H_
#define COMMANDBAN_H_

#include "Chat/Commands/ChatCommand.h"

namespace Chat
{

class CommandBan : public ChatCommand
{
public:
    virtual bool CheckSyntax(const std::vector<std::string>& splitedCommand) const override;
    virtual void ExecuteCommand(const CommandSender& sender, std::vector<std::string> splitedCommand) const override;
};

} /* namespace Chat */
#endif /* COMMANDBAN_H_ */
