#ifndef COMMANDSAVE_H_
#define COMMANDSAVE_H_

#include "Chat/Commands/ChatCommand.h"

namespace Chat
{

class CommandSave: public ChatCommand
{
public:
    virtual void ExecuteCommand(const CommandSender& sender, std::vector<std::string> splitedCommand) const override;
};

} /* namespace Chat */
#endif /* COMMANDSAVE_H_ */
