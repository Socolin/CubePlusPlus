#ifndef COMMANDDEOP_H_
#define COMMANDDEOP_H_

#include "Chat/Commands/ChatCommand.h"

namespace Chat
{

class CommandDeOp: public ChatCommand
{
public:
    CommandDeOp(const CommandSender& sender, const std::vector<std::string>& splitedCommand);
    virtual bool CheckSyntax() override;
    virtual void ExecuteCommand() override;
};

} /* namespace Chat */
#endif /* COMMANDDEOP_H_ */
