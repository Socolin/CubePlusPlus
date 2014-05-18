#ifndef COMMANDSTOP_H_
#define COMMANDSTOP_H_

#include "Chat/Commands/ChatCommand.h"

namespace Chat
{
class CommandStop: public ChatCommand
{
public:
    CommandStop(const CommandSender& sender, const std::vector<std::string>& splitedCommand);
    virtual void ExecuteCommand() override;
};

} /* namespace Chat */
#endif /* COMMANDSTOP_H_ */
