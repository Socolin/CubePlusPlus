#ifndef OPCODE_H_
#define OPCODE_H_

#include "NetworkSession.h"

namespace Network
{
enum eSessionState
{
	STATE_NEVER,
	STATE_EVERYTIME,
	STATE_NOTLOGGED,
	STATE_LOGGING,
	STATE_INGAME
};

#define TOTAL_OPCODE_COUNT 256

/// Structure permettant de lié un Opcode a une fonction
struct OpcodeHandler
{
	OpcodeHandler()
	{
		name = "UNK_OPCODE";
		state = STATE_NEVER;
		handler = NULL;
	}
	OpcodeHandler(char const* name,eSessionState state,void (NetworkSession::*handler)())
	{
		this->name = name;
		this->state = state;
		this->handler = handler;
	}
	/// Nom de l'opcode
	char const* name;
	/// Etat de la session requis pour que l'opcode soit réceptionné
	eSessionState state;
	/// Méthode associé a l'opcode
	void (NetworkSession::*handler)() throw (NetworkException);
};

extern OpcodeHandler opcodeTable[TOTAL_OPCODE_COUNT];
void initOpcode();
}
#endif
