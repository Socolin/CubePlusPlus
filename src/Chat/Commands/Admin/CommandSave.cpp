#include "CommandSave.h"

#include "World/WorldManager.h"

namespace Chat
{

void CommandSave::ExecuteCommand(const CommandSender& sender, std::vector<std::string> /*splitedCommand*/) const
{
    World::World* world = getWorldFromSender(sender);
    sender.chatStream << COLOR_SYSTEM << "Saving world" << std::endl;
}

} /* namespace Chat */
