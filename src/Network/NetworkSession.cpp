#include "NetworkSession.h"
#include "NetworkException.h"
#include "NetworkPacket.h"

#include <iostream>
#include <unistd.h>
#include "Opcode.h"

namespace Network
{
NetworkSession::NetworkSession(int socket) :
		socket(socket), buffer(INITIAL_BUFFER_SIZE),lastKeepAliveTick(0),startPosInBuffer(0),bufferSize(0),maxBufferSize(0)
{

}

NetworkSession::~NetworkSession()
{
}

void NetworkSession::ReceiveInBuffer() throw (NetworkException)
{
	int count = 0;
	startPosInBuffer = 0;
	bufferSize = 0;
	do
	{
		if (maxBufferSize < bufferSize + INITIAL_BUFFER_SIZE)
			buffer.resize(maxBufferSize + INITIAL_BUFFER_SIZE);
		count = read(socket, &(buffer[startPosInBuffer]), INITIAL_BUFFER_SIZE);
		if (count == -1)
		{
			throw NetworkException("Read == -1");
		}
		bufferSize += count;
	}
	while (count == INITIAL_BUFFER_SIZE);
}
void NetworkSession::ReceiveData() throw (NetworkException)
{
	ReceiveInBuffer();
	while (startPosInBuffer < bufferSize)
	{
		unsigned char packetId = readByte();
		const OpcodeHandler& handler = opcodeTable[packetId];
		std::cout << "Receive packet:"<< opcodeTable[packetId].name << " " << ((int)(packetId)&0xff) << std::endl;
		if (handler.state == STATE_NEVER)
		{
			throw NetworkException("Receive bad packet id");
		}
		(this->*handler.handler) ();
	}
}

void NetworkSession::readData(int length, char* data) throw (NetworkException)
{
	int count;
	startPosInBuffer = 0;
	count = read(socket, &(buffer[0]), length);

	if (count == -1)
		throw NetworkException("Error while reading data");
	if (count == 0)
		throw NetworkException("Disconnect");
	if (count < length)
		throw NetworkException("Not enough data");
}

char NetworkSession::readByte() throw (NetworkException)
{
	if (1 + startPosInBuffer > bufferSize)
		throw NetworkException("1 + startPosInBuffer > bufferSize");

	return buffer[startPosInBuffer++];
}

short NetworkSession::readShort() throw (NetworkException)
{
	if (2 + startPosInBuffer > bufferSize)
		throw NetworkException("2 + startPosInBuffer > bufferSize");
	short result = 0;
	result =  (short(buffer[startPosInBuffer])<<8 & 0xFF00) |
			(short(buffer[startPosInBuffer + 1]) & 0x00FF);
	startPosInBuffer += 2;
	return result;
}
int NetworkSession::readInt() throw (NetworkException)
{
	if (4 + startPosInBuffer > bufferSize)
		throw NetworkException("4 + startPosInBuffer > bufferSize");

	int result = 0;
	result = buffer[startPosInBuffer] << 24;
	result += buffer[startPosInBuffer + 1] << 16;
	result += buffer[startPosInBuffer + 2] << 8;
	result += buffer[startPosInBuffer + 3];
	startPosInBuffer += 4;
	return result;
}

std::wstring NetworkSession::readString(int maxSize) throw (NetworkException)
{
	short length = readShort();
	if (length < 0)
		throw NetworkException("String length < 0");

	if (length > maxSize)
		throw NetworkException("String length > maxSize");

	if (length + startPosInBuffer > bufferSize)
		throw NetworkException("String length < 0");

	wchar_t utf16Text[length];
	for (int i = 0; i < length; i++)
		utf16Text[i] = (short(buffer[startPosInBuffer + (i * 2)]) << 8 & 0xFF00)
				| (short(buffer[startPosInBuffer + (i * 2 + 1)]) & 0x00FF);
	std::wstring utf16RealText(utf16Text, length);

	startPosInBuffer += length * 2;
	return utf16RealText;
}


void NetworkSession::handleKeepAlive() throw (NetworkException)
{
	int value = readInt();
	std::cout << "KeepAlive:" << value << std::endl;
}

void NetworkSession::handleHandShake() throw (NetworkException)
{
	std::wstring value = readString(512);

	std::wcout << L"str: size:"<< value.length() << L" value:\"" << value << L"\"" << std::endl;

	NetworkPacket packet;
	std::wstring str(L"-");
	unsigned char packetId = 2;
	packet << packetId << str;
	packet.SendPacket(socket);
}

}
