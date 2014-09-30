#ifndef OPCODE_H_
#define OPCODE_H_

#include "OpcodeList.h"
#include "NetworkSession.h"

namespace Network
{
/// Opcode handler
struct OpcodeHandler
{
    OpcodeHandler()
        : name("UNK_OPCODE")
        , state(STATE_NEVER)
        , handler(nullptr)
        , debug(true)
        , packetSize(0)
    {
    }
    OpcodeHandler(char const* name, int state, void (NetworkSession::*handler)() throw (NetworkException))
        : name(name)
        , state(state)
        , handler(handler)
        , debug(true)
        , packetSize(0)
    {
    }
    OpcodeHandler(char const* name, int state, void (NetworkSession::*handler)() throw (NetworkException), bool debug)
        : name(name)
        , state(state)
        , handler(handler)
        , debug(debug)
        , packetSize(0)
    {
    }
    OpcodeHandler(char const* name, int state, void (NetworkSession::*handler)() throw (NetworkException), int size)
        : name(name)
        , state(state)
        , handler(handler)
        , debug(true)
        , packetSize(size)
    {
    }
    OpcodeHandler(char const* name, int state, void (NetworkSession::*handler)() throw (NetworkException), bool debug, int size)
        : name(name)
        , state(state)
        , handler(handler)
        , debug(debug)
        , packetSize(size)
    {
    }

    /// Opcode name
    char const* name;
    /// State of session when this opcode is receive, if state is not valid, client is kick
    int state;
    /// Handler called on packet reception
    void (NetworkSession::*handler)() throw (NetworkException);
    /// Active or not debug message for this opcode
    bool debug;
    /// Minimum size needed in buffer for trying decode this opcode
    size_t packetSize;
};

extern OpcodeHandler opcodeTable[TOTAL_OPCODE_COUNT];
void initOpcode();
}
#endif
