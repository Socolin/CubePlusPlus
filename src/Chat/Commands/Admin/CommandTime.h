#ifndef COMMANDTIME_H_
#define COMMANDTIME_H_

#include "Chat/Commands/ChatCommand.h"

namespace Chat
{

class CommandTime : public ChatCommand
{
public:
    virtual void ExecuteCommand(const CommandSender& sender, std::vector<std::string> splitedCommand) const override;
};

} /* namespace Chat */
#endif /* COMMANDTIME_H_ */
