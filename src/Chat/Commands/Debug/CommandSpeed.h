#ifndef COMMANDSPEED_H_
#define COMMANDSPEED_H_

#include "Chat/Commands/ChatCommand.h"

namespace Chat
{

class CommandSpeed: public ChatCommand
{
public:
    CommandSpeed(const CommandSender& sender, const std::vector<std::string>& splitedCommand);
    virtual bool CheckSyntax() override;
    virtual void BadSyntaxMessage() const override;
protected:
    virtual void ExecuteCommandPlayer(World::EntityPlayer* plr) override;
};

} /* namespace Chat */
#endif /* COMMANDSPEED_H_ */
