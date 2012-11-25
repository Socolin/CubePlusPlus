#ifndef NETWORKSESSION_H_
#define NETWORKSESSION_H_

#include "NetworkException.h"
#include <SFML/System/Unicode.hpp>
#include <vector>

namespace Network
{
#define INITIAL_BUFFER_SIZE 512
#define MAX_TICK_FOR_KEEPALIVE 1200
class NetworkSession
{
public:
	NetworkSession(int socket);
	virtual ~NetworkSession();

	void ReceiveInBuffer() throw (NetworkException);
	void ReceiveData() throw (NetworkException);

	void handleKeepAlive() throw (NetworkException);
	void handleHandShake() throw (NetworkException);

	void updateTick(uint32_t tick)
	{
		if (lastKeepAliveTick - tick > MAX_TICK_FOR_KEEPALIVE)
		{
			// Disconnect
		}
	}
private:
	void readData(int length, char* data) throw (NetworkException);
	char readByte() throw (NetworkException);
	short readShort() throw (NetworkException);
	int readInt() throw (NetworkException);
	std::wstring readString(int maxSize) throw (NetworkException);

	int socket;
	std::vector<char> buffer;
	int lastKeepAliveTick;
	uint32_t startPosInBuffer;
	uint32_t bufferSize;
	uint32_t maxBufferSize;
};
}

#endif /* NETWORKSESSION_H_ */
