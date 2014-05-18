#include "CommandStop.h"

#include "World/WorldManager.h"

namespace Chat
{

CommandStop::CommandStop(const CommandSender& sender, const std::vector<std::string>& splitedCommand)
    : ChatCommand(sender, splitedCommand)
{
}

void CommandStop::ExecuteCommand()
{
    sender.chatStream << COLOR_SYSTEM << "Stopping the server" << std::endl;
    World::WorldManager::Instance().Stop();
}

} /* namespace Chat */
