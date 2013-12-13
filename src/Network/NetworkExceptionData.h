#ifndef NETWORKEXCEPTIONDATA_H_
#define NETWORKEXCEPTIONDATA_H_

#include "NetworkException.h"

namespace Network
{

class NetworkExceptionData : public NetworkException
{
public:
    NetworkExceptionData(const char* message);

};

} /* namespace Network */
#endif /* NETWORKEXCEPTIONDATA_H_ */
