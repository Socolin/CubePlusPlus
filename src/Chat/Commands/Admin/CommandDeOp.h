#ifndef COMMANDDEOP_H_
#define COMMANDDEOP_H_

#include "Chat/Commands/ChatCommand.h"

namespace Chat
{

class CommandDeOp: public ChatCommand
{
public:
    virtual bool CheckSyntax(const std::vector<std::string>& splitedCommand) const override;
    virtual void ExecuteCommand(const CommandSender& sender, std::vector<std::string> splitedCommand) const override;
};

} /* namespace Chat */
#endif /* COMMANDDEOP_H_ */
