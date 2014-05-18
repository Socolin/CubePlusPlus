#include "CommandReload.h"

#include "World/WorldManager.h"

namespace Chat
{

CommandReload::CommandReload(const CommandSender& sender, const std::vector<std::string>& splitedCommand)
    : ChatCommand(sender, splitedCommand)
{
}

void CommandReload::ExecuteCommand()
{
    sender.chatStream << COLOR_SYSTEM << "Reloading Ban list, Admin list and Whitelist..." << std::endl;
    World::WorldManager::Instance().Reload();
}

} /* namespace Chat */

