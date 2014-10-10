#ifndef COMMANDTELL_H_
#define COMMANDTELL_H_

#include "Chat/Commands/ChatCommand.h"

namespace Chat
{

class CommandTell: public ChatCommand
{
    enum eSyntax
    {
        SYNTAX_PLAYER_MSG,
        SYNTAX_ERROR
    };
public:
    CommandTell(const CommandSender& sender, const std::vector<std::string>& splitedCommand);
    virtual bool CheckSyntax() override;
    virtual void ExecuteCommandPlayer(World::EntityPlayer* plr) override;
    virtual bool CanUse() override;
    virtual void BadSyntaxMessage() const override;
private:
    eSyntax syntax;
};

} /* namespace Chat */
#endif /* COMMANDTELL_H_ */
