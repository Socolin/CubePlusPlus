/*
 * NetworkException.cpp
 *
 *  Created on: 21 nov. 2012
 *      Author: bertrand
 */

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
