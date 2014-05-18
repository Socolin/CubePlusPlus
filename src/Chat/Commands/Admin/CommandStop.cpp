#include "CommandStop.h"

#include "World/WorldManager.h"

namespace Chat
{

void Chat::CommandStop::ExecuteCommand(const CommandSender& sender, std::vector<std::string> splitedCommand) const
{
    sender.chatStream << COLOR_SYSTEM << "Stopping the server" << std::endl;
    World::WorldManager::Instance().Stop();
}

} /* namespace Chat */
