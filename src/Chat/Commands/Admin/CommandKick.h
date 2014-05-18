#ifndef COMMANDKICK_H_
#define COMMANDKICK_H_

#include "Chat/Commands/ChatCommand.h"

namespace Chat
{

class CommandKick: public ChatCommand
{
public:
    CommandKick(const CommandSender& sender, const std::vector<std::string>& splitedCommand);
    virtual bool CheckSyntax() override;
    virtual void ExecuteCommand() override;
};

} /* namespace Chat */
#endif /* COMMANDKICK_H_ */
