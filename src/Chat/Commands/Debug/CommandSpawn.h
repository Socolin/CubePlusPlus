#ifndef COMMANDSPAWN_H_
#define COMMANDSPAWN_H_

#include "Chat/Commands/ChatCommand.h"

namespace Chat
{

class CommandSpawn: public ChatCommand
{
    enum eSyntax
    {
        SYNTAX_ENTITY_STR,
        SYNTAX_ENTITY_STR_COUNT,
        SYNTAX_ENTITY_ID,
        SYNTAX_ENTITY_ID_COUNT,
        SYNTAX_ERROR
    };
public:
    CommandSpawn(const CommandSender& sender, const std::vector<std::string>& splitedCommand);
    virtual bool CheckSyntax() override;
    virtual void ExecuteCommand() override;
private:
    eSyntax syntax;
};

} /* namespace Chat */
#endif /* COMMANDSPAWN_H_ */
