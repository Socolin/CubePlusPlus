#ifndef COMMANDRELOAD_H_
#define COMMANDRELOAD_H_

#include "Chat/Commands/ChatCommand.h"

namespace Chat
{

class CommandReload: public ChatCommand
{
public:
    CommandReload(const CommandSender& sender, const std::vector<std::string>& splitedCommand);
    virtual void ExecuteCommand() override;
};

} /* namespace Chat */
#endif /* COMMANDRELOAD_H_ */
