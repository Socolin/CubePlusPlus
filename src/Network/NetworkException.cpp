#include "NetworkException.h"

namespace Network
{

NetworkException::NetworkException()
    : message("unk")
{
}

NetworkException::NetworkException(const char* message)
    : message(message)
{
}

} /* namespace Network */
