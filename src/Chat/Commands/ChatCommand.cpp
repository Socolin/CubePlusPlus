#include "ChatCommand.h"

#include "Chat/ChatStream.h"
#include "Entity/EntityPlayer.h"
#include "Util/AssertUtil.h"

namespace Chat
{

ChatCommand::ChatCommand()
{
}

ChatCommand::~ChatCommand()
{
}

bool ChatCommand::CheckSyntax(const std::vector<std::string>& /*splitedCommand*/) const
{
    return true;
}

void ChatCommand::BadSyntaxMessage(const CommandSender& sender) const
{
    (*sender.chatStream) << Chat::RED << "Bad syntax" << std::endl;
}

void ChatCommand::ExecuteCommand(const CommandSender& sender, std::vector<std::string> splitedCommand) const
{
    switch (sender.type)
    {
    case PLAYER:
        ExecuteCommandPlayer(sender.senderPtr.plr, sender.chatStream, splitedCommand);
        break;
    case COMMAND_BLOCK:
        break;
    case CONSOLE:
        ExecuteCommandConsole(sender.chatStream, splitedCommand);
        break;
    default:
        AssertSwitchBadDefault(sender.type);
        break;
    }
}

void ChatCommand::ExecuteCommandPlayer(World::EntityPlayer* /*plr*/, ChatStream* /*chatStream*/, std::vector<std::string> /*splitedCommand*/) const
{
}

void ChatCommand::ExecuteCommandConsole(ChatStream* /*chatStream*/, std::vector<std::string> /*splitedCommand*/) const
{
}

// "s:i:u:s"
bool ChatCommand::checkSyntaxtWith(const std::string& pattern, std::vector<std::string> splitedCommand) const
{
    std::istringstream patternStream(pattern);
    std::string type;
    size_t pos = 1;
    while (std::getline(patternStream, type, ':'))
    {
        if (pos >= splitedCommand.size()) {
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
        } else if (type == "s") { // string

        }
        pos++;
    }
    if (pos < splitedCommand.size())
    {
        return false;
    }
    return true;
}

} /* namespace Database */
