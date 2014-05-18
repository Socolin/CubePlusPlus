#ifndef COMMANDMUTE_H_
#define COMMANDMUTE_H_

#include "Chat/Commands/ChatCommand.h"

namespace Chat
{

class CommandMute: public ChatCommand
{
public:
    CommandMute(const CommandSender& sender, const std::vector<std::string>& splitedCommand);
    virtual bool CheckSyntax() override;
    virtual void ExecuteCommand() override;
};

} /* namespace Chat */
#endif /* COMMANDMUTE_H_ */
