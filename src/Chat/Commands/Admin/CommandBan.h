#ifndef COMMANDBAN_H_
#define COMMANDBAN_H_

#include "Chat/Commands/ChatCommand.h"

namespace Chat
{

class CommandBan: public ChatCommand
{
public:
    CommandBan(const CommandSender& sender, const std::vector<std::string>& splitedCommand);
    virtual bool CheckSyntax() override;
    virtual void ExecuteCommand() override;
};

} /* namespace Chat */
#endif /* COMMANDBAN_H_ */
