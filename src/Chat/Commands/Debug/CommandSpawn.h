#ifndef COMMANDSPAWN_H_
#define COMMANDSPAWN_H_

#include "Chat/Commands/ChatCommand.h"

namespace Chat
{

class CommandSpawn: public ChatCommand
{
public:
    virtual bool CheckSyntax(const std::vector<std::string>& splitedCommand) const override;
    virtual void ExecuteCommand(const CommandSender& sender, std::vector<std::string> splitedCommand) const override;
};

} /* namespace Chat */
#endif /* COMMANDSPAWN_H_ */
