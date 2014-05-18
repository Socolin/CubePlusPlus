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

    manager.RegisterChatCommand("ban",          &createChatCommand<CommandBan>,         ALL_SENDER);
    manager.RegisterChatCommand("unban",        &createChatCommand<CommandUnban>,       ALL_SENDER);
    manager.RegisterChatCommand("op",           &createChatCommand<CommandOp>,          ALL_SENDER);
    manager.RegisterChatCommand("deop",         &createChatCommand<CommandDeOp>,        ALL_SENDER);
    manager.RegisterChatCommand("admin",        &createChatCommand<CommandOp>,          ALL_SENDER);
    manager.RegisterChatCommand("unadmin",      &createChatCommand<CommandDeOp>,        ALL_SENDER);
    manager.RegisterChatCommand("kick",         &createChatCommand<CommandKick>,        ALL_SENDER);
    manager.RegisterChatCommand("mute",         &createChatCommand<CommandMute>,        ALL_SENDER);
    manager.RegisterChatCommand("unmute",       &createChatCommand<CommandUnmute>,      ALL_SENDER);
    manager.RegisterChatCommand("reload",       &createChatCommand<CommandReload>,      PLAYER | CONSOLE);
    manager.RegisterChatCommand("save",         &createChatCommand<CommandSave>,        ALL_SENDER);
    manager.RegisterChatCommand("stop",         &createChatCommand<CommandStop>,        PLAYER | CONSOLE);
    manager.RegisterChatCommand("tp",           &createChatCommand<CommandTeleport>,    ALL_SENDER);
    manager.RegisterChatCommand("time",         &createChatCommand<CommandTime>,        ALL_SENDER);
    manager.RegisterChatCommand("whitelist",    &createChatCommand<CommandWhitelist>,   ALL_SENDER);
}

} /* namespace Chat */
