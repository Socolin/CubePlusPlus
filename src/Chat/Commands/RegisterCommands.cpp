#include "RegisterCommands.h"

#include "ChatCommandManager.h"

#include "Admin/CommandBan.h"
#include "Admin/CommandChat.h"
#include "Admin/CommandDeOp.h"
#include "Admin/CommandGamemode.h"
#include "Admin/CommandKick.h"
#include "Admin/CommandMute.h"
#include "Admin/CommandOp.h"
#include "Admin/CommandReload.h"
#include "Admin/CommandSave.h"
#include "Admin/CommandStop.h"
#include "Admin/CommandTeleport.h"
#include "Admin/CommandTime.h"
#include "Admin/CommandUnban.h"
#include "Admin/CommandUnmute.h"
#include "Admin/CommandWhitelist.h"

#include "Debug/CommandSpeed.h"

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
    manager.RegisterChatCommand("gamemode",     &createChatCommand<CommandGamemode>,    ALL_SENDER);

    manager.RegisterChatCommand("speed",        &createChatCommand<CommandSpeed>,       PLAYER);
}

} /* namespace Chat */
