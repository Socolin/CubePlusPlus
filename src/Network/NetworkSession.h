#ifndef NETWORKSESSION_H_
#define NETWORKSESSION_H_

#include "NetworkException.h"
#include <SFML/System/Unicode.hpp>

namespace Network
{
#define MAX_BUFFER_SIZE 512

class NetworkSession
{
public:
	NetworkSession(int socket);
	virtual ~NetworkSession();

	void MarkReceiveData();
	void ReceiveData() throw (NetworkException);

	void handleKeepAlive() throw (NetworkException);
	void handleHandShake() throw (NetworkException);
private:
	void readData(int length, char* data) throw (NetworkException);
	char readByte() throw (NetworkException);
	short readShort() throw (NetworkException);
	int readInt() throw (NetworkException);
	std::wstring readString(int maxSize) throw (NetworkException);


	int socket;
	char buffer[MAX_BUFFER_SIZE];
	bool hasReceiveData;
};
}

#endif /* NETWORKSESSION_H_ */
