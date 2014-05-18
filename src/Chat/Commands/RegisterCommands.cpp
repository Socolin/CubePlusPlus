#include "RegisterCommands.h"

#include "ChatCommandManager.h"

#include "Chat/Commands/Admin/CommandBan.h"
#include "Chat/Commands/Admin/CommandChat.h"
#include "Chat/Commands/Admin/CommandDeOp.h"
#include "Chat/Commands/Admin/CommandKick.h"
#include "Chat/Commands/Admin/CommandMute.h"
#include "Chat/Commands/Admin/CommandOp.h"
#include "Chat/Commands/Admin/CommandReload.h"
#include "Chat/Commands/Admin/CommandSave.h"
#include "Chat/Commands/Admin/CommandStop.h"
#include "Chat/Commands/Admin/CommandTeleport.h"
#include "Chat/Commands/Admin/CommandTime.h"
#include "Chat/Commands/Admin/CommandUnban.h"
#include "Chat/Commands/Admin/CommandUnmute.h"
#include "Chat/Commands/Admin/CommandWhitelist.h"

namespace Chat
{
void RegisterCoreCommands()
{
    ChatCommandManager& manager = ChatCommandManager::Instance();

    manager.RegisterChatCommand("ban", new CommandBan());
    manager.RegisterChatCommand("unban", new CommandUnban());
    manager.RegisterChatCommand("op", new CommandOp());
    manager.RegisterChatCommand("deop", new CommandDeOp());
    manager.RegisterChatCommand("admin", new CommandOp());
    manager.RegisterChatCommand("unadmin", new CommandDeOp());
    manager.RegisterChatCommand("kick", new CommandKick());
    manager.RegisterChatCommand("mute", new CommandMute());
    manager.RegisterChatCommand("unmute", new CommandUnmute());
    manager.RegisterChatCommand("reload", new CommandReload());
    manager.RegisterChatCommand("save", new CommandSave());
    manager.RegisterChatCommand("stop", new CommandStop());
    manager.RegisterChatCommand("tp", new CommandTeleport());
    manager.RegisterChatCommand("time", new CommandTime());
    manager.RegisterChatCommand("whitelist", new CommandWhitelist());
}

} /* namespace Chat */
