#ifndef COMMANDSTOP_H_
#define COMMANDSTOP_H_

#include "Chat/Commands/ChatCommand.h"

namespace Chat
{
class CommandStop: public ChatCommand
{
public:
    virtual void ExecuteCommand(const CommandSender& sender, std::vector<std::string> splitedCommand) const override;
};

} /* namespace Chat */
#endif /* COMMANDSTOP_H_ */
