#ifndef COMMANDTIME_H_
#define COMMANDTIME_H_

#include "Chat/Commands/ChatCommand.h"

namespace Chat
{

class CommandTime: public ChatCommand
{
    enum eSyntax
    {
        SYNTAX_SUB_TIME,
        SYNTAX_DAYNIGHT,
        SYNTAX_ERROR
    };
public:
    CommandTime(const CommandSender& sender, const std::vector<std::string>& splitedCommand);
    virtual bool CheckSyntax() override;
    virtual void ExecuteCommand() override;
private:
    eSyntax syntax;
};

} /* namespace Chat */
#endif /* COMMANDTIME_H_ */
