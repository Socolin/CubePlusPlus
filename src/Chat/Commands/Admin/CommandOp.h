#ifndef COMMANDOP_H_
#define COMMANDOP_H_

#include "Chat/Commands/ChatCommand.h"

namespace Chat
{

class CommandOp : public ChatCommand
{
public:
    virtual bool CheckSyntax(const std::vector<std::string>& splitedCommand) const override;
    virtual void ExecuteCommand(const CommandSender& sender, std::vector<std::string> splitedCommand) const override;
};

} /* namespace Chat */
#endif /* COMMANDOP_H_ */
