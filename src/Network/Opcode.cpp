#include "NetworkSession.h"
#include "Opcode.h"

namespace Network
{

OpcodeHandler opcodeTable[TOTAL_OPCODE_COUNT];
void initOpcode()
{
	opcodeTable[OP_KEEPALIVE] = OpcodeHandler("KEEP_ALIVE", STATE_EVERYTIME, &NetworkSession::handleKeepAlive);
	opcodeTable[OP_HANDSHAKE] = OpcodeHandler("HANDSHAKE", STATE_NOTLOGGED, &NetworkSession::handleHandShake);
	opcodeTable[OP_CHAT_MESSAGE] = OpcodeHandler("CHAT_MESSAGE", STATE_LOGGED, &NetworkSession::handleChatMessage);
	opcodeTable[OP_USE_ENTITY] = OpcodeHandler("USE_ENTITY", STATE_LOGGED, &NetworkSession::handleUseEntity);
	opcodeTable[OP_PLAYER_POSITION] = OpcodeHandler("PLAYER_POSITION", STATE_LOGGED, &NetworkSession::handlePlayerPosition);
	opcodeTable[OP_PLAYER_LOOK] = OpcodeHandler("PLAYER_LOOK", STATE_LOGGED, &NetworkSession::handlePlayerLook);
	opcodeTable[OP_PLAYER_POSITION_AND_LOOK] = OpcodeHandler("POSITION_AND_LOOK", STATE_LOGGED, &NetworkSession::handlePlayerPositionAndLook);
	opcodeTable[OP_CLIENT_SETTINGS] = OpcodeHandler("CLIENT_SETTINGS", STATE_LOGGED, &NetworkSession::handleClientSettings);
	opcodeTable[OP_CLIENT_STATUSES] = OpcodeHandler("CLIENT_STATUSES", STATE_LOGGED, &NetworkSession::handleClientStatuses);
	opcodeTable[OP_ENCRYPTION_KEY_RESPONSE] = OpcodeHandler("ENCRYPTION_KEY_RESPONSE", STATE_LOGGING, &NetworkSession::handleEncryptionKeyResponse);
	opcodeTable[OP_ENCRYPTION_KEY_REQUEST] = OpcodeHandler("ENCRYPTION_KEY_REQUEST", STATE_NEVER, &NetworkSession::handleEncryptionKeyRequest);
	opcodeTable[OP_SERVER_LIST_PING] = OpcodeHandler("SERVER_LIST_PING", STATE_NOTLOGGED, &NetworkSession::handlePing);
}
} /* namespace Network */
