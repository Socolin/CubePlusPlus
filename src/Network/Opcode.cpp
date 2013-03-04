#include "NetworkSession.h"
#include "Opcode.h"

namespace Network
{

OpcodeHandler opcodeTable[TOTAL_OPCODE_COUNT];
void initOpcode()
{
    opcodeTable[OP_KEEPALIVE] = OpcodeHandler("KEEP_ALIVE", STATE_EVERYTIME, &NetworkSession::handleKeepAlive, false);
    opcodeTable[OP_HANDSHAKE] = OpcodeHandler("HANDSHAKE", STATE_NOTLOGGED, &NetworkSession::handleHandShake);
    opcodeTable[OP_CHAT_MESSAGE] = OpcodeHandler("CHAT_MESSAGE", STATE_INGAME, &NetworkSession::handleChatMessage);
    opcodeTable[OP_USE_ENTITY] = OpcodeHandler("USE_ENTITY", STATE_INGAME, &NetworkSession::handleUseEntity);
    opcodeTable[OP_PLAYER] = OpcodeHandler("PLAYER", STATE_INGAME, &NetworkSession::handlePlayer, false);
    opcodeTable[OP_PLAYER_POSITION] = OpcodeHandler("PLAYER_POSITION", STATE_INGAME, &NetworkSession::handlePlayerPosition, false);
    opcodeTable[OP_PLAYER_LOOK] = OpcodeHandler("PLAYER_LOOK", STATE_INGAME, &NetworkSession::handlePlayerLook, false);
    opcodeTable[OP_PLAYER_POSITION_AND_LOOK] = OpcodeHandler("POSITION_AND_LOOK", STATE_INGAME, &NetworkSession::handlePlayerPositionAndLook, false);
    opcodeTable[OP_PLAYER_DIGGING] = OpcodeHandler("PLAYER_DIGGING", STATE_INGAME, &NetworkSession::handlePlayerDigging);
    opcodeTable[OP_PLAYER_BLOCK_PLACEMENT] = OpcodeHandler("PLAYER_BLOCK_PLACEMENT", STATE_INGAME, &NetworkSession::handlePlayerBlockPlacement);
    opcodeTable[OP_HELD_ITEM_CHANGE] = OpcodeHandler("HELD_ITEM_CHANGE", STATE_INGAME, &NetworkSession::handleHeldItemChange);
    opcodeTable[OP_ANIMATION] = OpcodeHandler("ANIMATION", STATE_INGAME, &NetworkSession::handleAnimation);
    opcodeTable[OP_ENTITY_ACTION] = OpcodeHandler("ENTITY_ACTION", STATE_INGAME, &NetworkSession::handleEntityAction);
    opcodeTable[OP_CLOSE_WINDOW] = OpcodeHandler("CLOSE_WINDOW", STATE_INGAME, &NetworkSession::handleCloseWindow);
    opcodeTable[OP_CLICK_WINDOW] = OpcodeHandler("CLICK_WINDOW", STATE_INGAME, &NetworkSession::handleClickWindow);
    opcodeTable[OP_CONFIRM_TRANSACTION] = OpcodeHandler("CONFIRM_TRANSACTION", STATE_INGAME, &NetworkSession::handleConfirmTransaction);
    opcodeTable[OP_CREATIVE_INVENTORY_ACTION] = OpcodeHandler("CREATIVE_INVENTORY_ACTION", STATE_INGAME, &NetworkSession::handleCreativeInventoryAction);
    opcodeTable[OP_ENCHANT_ITEM] = OpcodeHandler("ENCHANT_ITEM", STATE_INGAME, &NetworkSession::handleEnchantItem);
    opcodeTable[OP_UPDATE_SIGN] = OpcodeHandler("UPDATE_SIGN", STATE_INGAME, &NetworkSession::handleUpdateSign);
    opcodeTable[OP_PLAYER_ABILITIES] = OpcodeHandler("PLAYER_ABILITIES", STATE_INGAME, &NetworkSession::handlePlayerAbilities);
    opcodeTable[OP_TAB_COMPLETE] = OpcodeHandler("TAB_COMPLETE", STATE_INGAME, &NetworkSession::handleTabComplete);
    opcodeTable[OP_CLIENT_SETTINGS] = OpcodeHandler("CLIENT_SETTINGS", STATE_INGAME, &NetworkSession::handleClientSettings);
    opcodeTable[OP_CLIENT_STATUSES] = OpcodeHandler("CLIENT_STATUSES", (STATE_INGAME | STATE_LOGGING), &NetworkSession::handleClientStatuses);
    opcodeTable[OP_PLUGIN_MESSAGE] = OpcodeHandler("PLUGIN_MESSAGE", STATE_INGAME, &NetworkSession::handlePluginMessage);
    opcodeTable[OP_ENCRYPTION_KEY_RESPONSE] = OpcodeHandler("ENCRYPTION_KEY_RESPONSE", STATE_NOTLOGGED, &NetworkSession::handleEncryptionKeyResponse);
    opcodeTable[OP_ENCRYPTION_KEY_REQUEST] = OpcodeHandler("ENCRYPTION_KEY_REQUEST", STATE_LOGGING, &NetworkSession::handleEncryptionKeyRequest);
    opcodeTable[OP_SERVER_LIST_PING] = OpcodeHandler("SERVER_LIST_PING", STATE_NOTLOGGED, &NetworkSession::handlePing);
    opcodeTable[OP_KICK] = OpcodeHandler("KICK", STATE_INGAME, &NetworkSession::handleDisconnect);
}
} /* namespace Network */
