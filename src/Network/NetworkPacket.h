/*
 * NetworkPacket.h
 *
 *  Created on: 22 nov. 2012
 *      Author: bertrand
 */

#ifndef NETWORKPACKET_H_
#define NETWORKPACKET_H_

#include <iostream>
#include <vector>

namespace Network
{

class NetworkPacket
{
public:
	NetworkPacket();
	NetworkPacket(size_t size);
	virtual ~NetworkPacket();

	void append(const void* data, std::size_t size);

	NetworkPacket& operator <<(bool& value);
	NetworkPacket& operator <<(char& value);
	NetworkPacket& operator <<(unsigned char& value);
	NetworkPacket& operator <<(short& value);
	NetworkPacket& operator <<(int& value);
	NetworkPacket& operator <<(long& value);
	NetworkPacket& operator <<(float& value);
	NetworkPacket& operator <<(double& value);
	NetworkPacket& operator <<(std::wstring& value);
	NetworkPacket& operator <<(const std::pair<char*, short>&);

	void SendPacket(int socket);

	const std::vector<char>& getPacketData() const
	{
		return packetData;
	}

	size_t getPacketSize() const
	{
		return packetSize;
	}


private:
	size_t bufferSize;
	std::vector<char> packetData;
	size_t packetSize;
	char tmpBuffer[8]; // Buffer pour permettre d'écrire tout les type d'int sans avoir à réalouer de la place à chaque fois
};

} /* namespace Network */
#endif /* NETWORKPACKET_H_ */
