#include "RegisterCommands.h"

#include "ChatCommandManager.h"

#include "Admin/CommandBan.h"

namespace Chat
{
void RegisterCoreCommands()
{
    ChatCommandManager& manager = ChatCommandManager::Instance();

    manager.RegisterChatCommand("ban", new CommandBan());
}

} /* namespace Chat */
