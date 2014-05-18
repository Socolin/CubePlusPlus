#include "CommandSave.h"

#include "World/World.h"
#include "World/WorldManager.h"

namespace Chat
{

CommandSave::CommandSave(const CommandSender& sender, const std::vector<std::string>& splitedCommand)
    : ChatCommand(sender, splitedCommand)
{
}

void CommandSave::ExecuteCommand()
{
    World::World* world = getWorldFromSender();
    world->Save();
    sender.chatStream << COLOR_SYSTEM << "Saving world" << std::endl;
}

} /* namespace Chat */
