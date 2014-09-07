#ifndef COMMANDWHITELIST_H_
#define COMMANDWHITELIST_H_

#include "Chat/Commands/ChatCommand.h"

namespace Chat
{

class CommandWhitelist: public ChatCommand
{
    enum eSyntax
    {
        SYNTAX_SUB,
        SYNTAX_SUB_PLR,
        SYNTAX_ERROR,
    };
public:
    CommandWhitelist(const CommandSender& sender, const std::vector<std::string>& splitedCommand);
    virtual bool CheckSyntax() override;
    virtual void ExecuteCommand() override;
    virtual void BadSyntaxMessage() const override;
private:
    eSyntax syntax;
};

} /* namespace Chat */
#endif /* COMMANDWHITELIST_H_ */
