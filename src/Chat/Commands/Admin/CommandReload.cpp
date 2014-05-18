#include "CommandReload.h"

#include "World/WorldManager.h"

namespace Chat
{

void CommandReload::ExecuteCommand(const CommandSender& sender, std::vector<std::string> /*splitedCommand*/) const
{
    sender.chatStream << COLOR_SYSTEM << "Reloading Ban list, Admin list and Whitelist..." << std::endl;
    World::WorldManager::Instance().Reload();
}

} /* namespace Chat */

