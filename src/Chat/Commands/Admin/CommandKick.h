#ifndef COMMANDKICK_H_
#define COMMANDKICK_H_

#include "Chat/Commands/ChatCommand.h"

namespace Chat
{

class CommandKick : public ChatCommand
{
public:
    virtual bool CheckSyntax(const std::vector<std::string>& splitedCommand) const override;
    virtual void ExecuteCommand(const CommandSender& sender, std::vector<std::string> splitedCommand) const override;
};

} /* namespace Chat */
#endif /* COMMANDKICK_H_ */
