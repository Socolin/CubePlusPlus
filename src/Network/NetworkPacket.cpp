/*
 * NetworkPacket.cpp
 *
 *  Created on: 22 nov. 2012
 *      Author: bertrand
 */

#include "NetworkPacket.h"

#include <cstring>
#include <sys/socket.h>

namespace Network
{

NetworkPacket::NetworkPacket() :
		bufferSize(32), packetData(32), packetSize(0)
{

}
NetworkPacket::NetworkPacket(size_t minSize) :
		bufferSize(minSize), packetData(minSize), packetSize(0)
{

}
NetworkPacket::~NetworkPacket()
{
}

void NetworkPacket::append(const void* data, std::size_t dataSize)
{
	if (packetSize + dataSize > bufferSize)
	{
		packetData.resize(packetSize + dataSize);
	}

	std::memcpy(&packetData[packetSize], data, dataSize);
	packetSize += dataSize;
}

NetworkPacket& NetworkPacket::operator <<(bool& value)
{
	append(&value, 1);
	return *this;
}
NetworkPacket& NetworkPacket::operator <<(char& value)
{
	append(&value, 1);
	return *this;
}
NetworkPacket& NetworkPacket::operator <<(unsigned char& value)
{
	append(&value, 1);
	return *this;
}

NetworkPacket& NetworkPacket::operator <<(short & value)
{
	tmpBuffer[0] = (value >> 8) & 0xff;
	tmpBuffer[1] = value & 0xff;
	append(tmpBuffer, 2);
	return *this;
}

NetworkPacket& NetworkPacket::operator <<(int& value)
{
	tmpBuffer[0] = (value >> 24) & 0xff;
	tmpBuffer[1] = (value >> 16) & 0xff;
	tmpBuffer[2] = (value >> 8) & 0xff;
	tmpBuffer[3] = value & 0xff;
	append(tmpBuffer, 4);
	return *this;
}

NetworkPacket& NetworkPacket::operator <<(long & value)
{
	tmpBuffer[0] = (value >> 56) & 0xff;
	tmpBuffer[1] = (value >> 48) & 0xff;
	tmpBuffer[2] = (value >> 40) & 0xff;
	tmpBuffer[3] = (value >> 32) & 0xff;
	tmpBuffer[4] = (value >> 24) & 0xff;
	tmpBuffer[5] = (value >> 16) & 0xff;
	tmpBuffer[6] = (value >> 8) & 0xff;
	tmpBuffer[7] = value & 0xff;
	append(tmpBuffer, 4);
	return *this;
}

NetworkPacket& NetworkPacket::operator <<(float& value)
{
	return *this;
}

NetworkPacket& NetworkPacket::operator <<(double& value)
{
	return *this;
}

NetworkPacket& NetworkPacket::operator <<(std::wstring& value)
{
	short strLenght = value.size();
	tmpBuffer[0] = (strLenght >> 8) & 0xff;
	tmpBuffer[1] = strLenght & 0xff;
	append(tmpBuffer, 2);
	const wchar_t* datastr = value.c_str();
	for (short i = 0; i < strLenght; i++)
	{
		tmpBuffer[0] = (datastr[i] >> 8) & 0xff;
		tmpBuffer[1] = datastr[i] & 0xff;
		append(tmpBuffer, 2);
	}
	return *this;
}

void NetworkPacket::SendPacket(int socket)
{
	send(socket, &packetData[0], packetSize, 0);
}

} /* namespace Network */
