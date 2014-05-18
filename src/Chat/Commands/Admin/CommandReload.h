#ifndef COMMANDRELOAD_H_
#define COMMANDRELOAD_H_

#include "Chat/Commands/ChatCommand.h"

namespace Chat
{

class CommandReload: public ChatCommand
{
public:
    virtual void ExecuteCommand(const CommandSender& sender, std::vector<std::string> splitedCommand) const override;
};

} /* namespace Chat */
#endif /* COMMANDRELOAD_H_ */
