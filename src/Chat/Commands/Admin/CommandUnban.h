#ifndef COMMANDUNBAN_H_
#define COMMANDUNBAN_H_

#include "Chat/Commands/ChatCommand.h"

namespace Chat
{

class CommandUnban: public ChatCommand
{
public:
    CommandUnban(const CommandSender& sender, const std::vector<std::string>& splitedCommand);
    virtual bool CheckSyntax() override;
    virtual void ExecuteCommand() override;
};

} /* namespace Chat */
#endif /* COMMANDUNBAN_H_ */
