#ifndef OPCODE_H_
#define OPCODE_H_

#include "OpcodeList.h"
#include "NetworkSession.h"

namespace Network
{

/// Structure permettant de lié un Opcode a une fonction
struct OpcodeHandler
{
	OpcodeHandler()
	{
		name = "UNK_OPCODE";
		state = STATE_NEVER;
		handler = NULL;
	}
	OpcodeHandler(char const* name,int state,void (NetworkSession::*handler)())
	{
		this->name = name;
		this->state = state;
		this->handler = handler;
	}
	/// Nom de l'opcode
	char const* name;
	/// Etat de la session requis pour que l'opcode soit réceptionné
	int state;
	/// Méthode associé a l'opcode
	void (NetworkSession::*handler)() throw (NetworkException);
};

extern OpcodeHandler opcodeTable[TOTAL_OPCODE_COUNT];
void initOpcode();
}
#endif
