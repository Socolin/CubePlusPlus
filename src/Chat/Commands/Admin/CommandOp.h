#ifndef COMMANDOP_H_
#define COMMANDOP_H_

#include "Chat/Commands/ChatCommand.h"

namespace Chat
{

class CommandOp: public ChatCommand
{
public:
    CommandOp(const CommandSender& sender, const std::vector<std::string>& splitedCommand);
    virtual bool CheckSyntax() override;
    virtual void ExecuteCommand() override;
};

} /* namespace Chat */
#endif /* COMMANDOP_H_ */
