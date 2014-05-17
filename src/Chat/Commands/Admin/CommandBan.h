#ifndef COMMANDBAN_H_
#define COMMANDBAN_H_

#include "Chat/Commands/ChatCommand.h"

namespace Chat
{

class CommandBan : public ChatCommand
{
public:
    CommandBan();
    virtual ~CommandBan();
    virtual bool CheckSyntax(const std::vector<std::string>& splitedCommand) const override;
    virtual void ExecuteCommand(const CommandSender& sender, std::vector<std::string> splitedCommand) const override;
protected:
};

} /* namespace Chat */
#endif /* COMMANDBAN_H_ */
