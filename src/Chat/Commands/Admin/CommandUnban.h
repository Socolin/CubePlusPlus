#ifndef COMMANDUNBAN_H_
#define COMMANDUNBAN_H_

#include "Chat/Commands/ChatCommand.h"

namespace Chat
{

class CommandUnban: public ChatCommand
{
public:
    virtual bool CheckSyntax(const std::vector<std::string>& splitedCommand) const override;
    virtual void ExecuteCommand(const CommandSender& sender, std::vector<std::string> splitedCommand) const override;
};

} /* namespace Chat */
#endif /* COMMANDUNBAN_H_ */
