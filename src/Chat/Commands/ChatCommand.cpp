#include "ChatCommand.h"

#include "Chat/ChatStream.h"
#include "Entity/EntityPlayer.h"
#include "Util/AssertUtil.h"
#include "World/WorldManager.h"

namespace Chat
{

ChatCommand::ChatCommand(const CommandSender& sender, const std::vector<std::string>& splitedCommand)
    : sender(sender)
    , splitedCommand(splitedCommand)
{
}

ChatCommand::~ChatCommand()
{
}

bool ChatCommand::CheckSyntax()
{
    return true;
}

void ChatCommand::BadSyntaxMessage() const
{
    sender.chatStream << RED << "Bad syntax" << std::endl;
}

void ChatCommand::ExecuteCommand()
{
    switch (sender.type)
    {
    case PLAYER:
        ExecuteCommandPlayer(sender.senderPtr.plr);
        break;
    case COMMAND_BLOCK:
        break;
    case CONSOLE:
        ExecuteCommandConsole();
        break;
    default:
        AssertSwitchBadDefault(sender.type);
        break;
    }
}

void ChatCommand::ExecuteCommandPlayer(World::EntityPlayer* /*plr*/)
{
}

void ChatCommand::ExecuteCommandConsole()
{
}

bool ChatCommand::CanUse()
{
    switch (sender.type)
    {
    case COMMAND_BLOCK:
        return true;
    case CONSOLE:
        return true;
    case PLAYER:
        // FIXME need permission and group system
        return sender.senderPtr.plr->isAdmin();
    default:
        AssertSwitchBadDefault(sender.type);
        break;
    }
    return false;
}

// "s:i:u:s"
bool ChatCommand::checkSyntaxtWith(const std::string& pattern) const
{
    std::istringstream patternStream(pattern);
    std::string type;
    size_t pos = 1;
    while (std::getline(patternStream, type, ':'))
    {
        if (pos >= splitedCommand.size()) {
            return false;
        }

        if (type.empty()) {
            return false;
        }

        std::string s = splitedCommand[pos];
        if (type == "i") {
            for (size_t i = 0; i < s.size(); i++)
            {
                if (s[i] == '-')
                {
                    if (i > 0)
                    {
                        return false;
                    }
                    else
                    {
                        if (s.size() == 1)
                        {
                            return false;
                        }
                    }
                }
                else
                {
                    if (!isdigit(s[i]))
                    {
                        return false;
                    }
                }
            }
        } else if (type == "u") { // unsigned int
            for (size_t i = 0; i < s.size(); i++)
            {
                if (!isdigit(s[i]))
                {
                    return false;
                }
            }
        } else if (type == "f") { // float
            bool pointFound = false;
            for (size_t i = 0; i < s.size(); i++)
            {
                if (s[i] == '-')
                {
                    if (i > 0)
                    {
                        return false;
                    }
                    else
                    {
                        if (s.size() == 1)
                        {
                            return false;
                        }
                    }
                }
                else if (s[i] == '.') {
                    if (pointFound) {
                        return false;
                    }
                    pointFound = true;
                }
                else
                {
                    if (!isdigit(s[i]))
                    {
                        return false;
                    }
                }
            }
        } else if (type[0] == 's') { // string

        } else if (type[0] == 'p') { // playername
            //FIXME regex ?

        }
        pos++;
    }
    if (pos < splitedCommand.size())
    {
        return false;
    }
    return true;
}

World::World* ChatCommand::getWorldFromSender() const
{
    World::World* world = nullptr;

    switch (sender.type)
    {
    case COMMAND_BLOCK:
        break;
    case CONSOLE:
        world = World::WorldManager::Instance().GetWorld();
        break;
    case PLAYER:
        world = sender.senderPtr.plr->GetWorld();
        break;
    default:
        AssertSwitchBadDefault(sender.type);
        break;
    }

    return world;
}

} /* namespace Database */
