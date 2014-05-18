#ifndef COMMANDSAVE_H_
#define COMMANDSAVE_H_

#include "Chat/Commands/ChatCommand.h"

namespace Chat
{

class CommandSave: public ChatCommand
{
public:
    CommandSave(const CommandSender& sender, const std::vector<std::string>& splitedCommand);
    virtual void ExecuteCommand() override;
};

} /* namespace Chat */
#endif /* COMMANDSAVE_H_ */
