#include <Chat/Commands/Debug/CommandSpawn.h>

namespace Chat
{

CommandSpawn::CommandSpawn(const CommandSender& sender, const std::vector<std::string>& splitedCommand)
    : ChatCommand(sender, splitedCommand)
{
}

bool CommandSpawn::CheckSyntax()
{
    //FIXME
    return false;
}

void CommandSpawn::ExecuteCommand()
{
    //FIXME
}

} /* namespace Chat */

