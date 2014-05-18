#ifndef COMMANDWHITELIST_H_
#define COMMANDWHITELIST_H_

#include "Chat/Commands/ChatCommand.h"

namespace Chat
{

class CommandWhitelist: public ChatCommand
{
public:
    virtual bool CheckSyntax(const std::vector<std::string>& splitedCommand) const override;
    virtual void ExecuteCommand(const CommandSender& sender, std::vector<std::string> splitedCommand) const override;
};

} /* namespace Chat */
#endif /* COMMANDWHITELIST_H_ */
