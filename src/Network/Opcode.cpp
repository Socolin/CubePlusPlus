#include "NetworkSession.h"
#include "Opcode.h"

namespace Network
{

OpcodeHandler opcodeTable[TOTAL_OPCODE_COUNT];
void initOpcode()
{
	opcodeTable[0] = OpcodeHandler("KEEP_ALIVE", STATE_EVERYTIME,
			&NetworkSession::handleKeepAlive);
	opcodeTable[2] = OpcodeHandler("HANDSHAKE", STATE_NOTLOGGED,
			&NetworkSession::handleHandShake);
}
} /* namespace Network */
