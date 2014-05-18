#ifndef COMMANDSPAWN_H_
#define COMMANDSPAWN_H_

#include "Chat/Commands/ChatCommand.h"

namespace Chat
{

class CommandSpawn: public ChatCommand
{
public:
    CommandSpawn(const CommandSender& sender, const std::vector<std::string>& splitedCommand);
    virtual bool CheckSyntax() override;
    virtual void ExecuteCommand() override;
};

} /* namespace Chat */
#endif /* COMMANDSPAWN_H_ */
