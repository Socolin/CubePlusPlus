#ifndef COMMANDUNMUTE_H_
#define COMMANDUNMUTE_H_

#include "Chat/Commands/ChatCommand.h"

namespace Chat
{

class CommandUnmute: public ChatCommand
{
public:
    CommandUnmute(const CommandSender& sender, const std::vector<std::string>& splitedCommand);
    virtual bool CheckSyntax() override;
    virtual void ExecuteCommand() override;
};

} /* namespace Chat */
#endif /* COMMANDUNMUTE_H_ */
