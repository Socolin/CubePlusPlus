#ifndef COMMANDUNMUTE_H_
#define COMMANDUNMUTE_H_

#include "Chat/Commands/ChatCommand.h"

namespace Chat
{

class CommandUnmute: public ChatCommand
{
public:
    virtual bool CheckSyntax(const std::vector<std::string>& splitedCommand) const override;
    virtual void ExecuteCommand(const CommandSender& sender, std::vector<std::string> splitedCommand) const override;
};

} /* namespace Chat */
#endif /* COMMANDUNMUTE_H_ */
