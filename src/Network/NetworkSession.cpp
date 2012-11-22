#include "NetworkSession.h"
#include "NetworkException.h"
#include "NetworkPacket.h"

#include <iostream>
#include <unistd.h>
#include "Opcode.h"

namespace Network
{
NetworkSession::NetworkSession(int socket) :
		socket(socket), hasReceiveData(false)
{

}

NetworkSession::~NetworkSession()
{
}

void NetworkSession::MarkReceiveData()
{
	hasReceiveData = true;
}

void NetworkSession::ReceiveData() throw (NetworkException)
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

void NetworkSession::readData(int length, char* data) throw (NetworkException)
{
	int count;
	count = read(socket, buffer, length); //TODO: Bufferisé

	if (count == -1)
		throw NetworkException("Error while reading data");
	if (count == 0)
		throw NetworkException("Disconnect");
	if (count < length)
		throw NetworkException("Not enough data");
}

char NetworkSession::readByte() throw (NetworkException)
{
	readData(1,buffer);
	return buffer[0];
}

short NetworkSession::readShort() throw (NetworkException)
{
	short result = 0;
	readData(2,buffer);
	result =  (short(buffer[0])<<8 & 0xFF00) |
			(short(buffer[1]) & 0x00FF);
	return result;
}
int NetworkSession::readInt() throw (NetworkException)
{
	int result = 0;
	readData(4,buffer);
	result = buffer[0] << 24;
	result += buffer[1] << 16;
	result += buffer[2] << 8;
	result += buffer[3];
	return result;
}

std::wstring NetworkSession::readString(int maxSize) throw (NetworkException)
{
	short length = readShort();
	if (length < 0)
		throw NetworkException("String length < 0");

	if (length > maxSize)
		throw NetworkException("String length > maxSize");

	if (length * 2 > MAX_BUFFER_SIZE)
		throw NetworkException("String length > MAX_BUFFER_SIZE");

	readData(length * 2, buffer);
	wchar_t utf16Text[length];
	for (int i = 0; i < length; i++)
		utf16Text[i] = (short(buffer[i * 2]) << 8 & 0xFF00)
				| (short(buffer[i * 2 + 1]) & 0x00FF);
	std::wstring utf16RealText (utf16Text,length);

	return utf16RealText;
}


void NetworkSession::handleKeepAlive() throw (NetworkException)
{
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
