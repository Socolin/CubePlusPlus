#include "NetworkSession.h"
#include "NetworkException.h"
#include "NetworkPacket.h"

#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include "Opcode.h"

namespace Network
{
NetworkSession::NetworkSession(int socket) :
		socket(socket), buffer(INITIAL_BUFFER_SIZE),lastKeepAliveTick(0),startPosInBuffer(0),bufferSize(0),maxBufferSize(0),cryptedMode(false),cfbDecryptor(NULL),cfbEncryptor(NULL),aesDecryptor(NULL),aesEncryptor(NULL)
{

}

NetworkSession::~NetworkSession()
{
	if (cryptedMode)
	{
		delete cfbDecryptor;
		delete cfbEncryptor;
		delete aesDecryptor;
		delete aesEncryptor;
	}
}

void NetworkSession::ReceiveInBuffer() throw (NetworkException)
{
	int count = 0;
	startPosInBuffer = 0;
	bufferSize = 0;
	do
	{
		if (maxBufferSize < bufferSize + INITIAL_BUFFER_SIZE)
		{
			buffer.resize(maxBufferSize + INITIAL_BUFFER_SIZE);
		}
		count = read(socket, &(buffer[startPosInBuffer]), INITIAL_BUFFER_SIZE);
		if (count == -1)
		{
			throw NetworkException("Read == -1");
		}
		bufferSize += count;
	}
	while (count == INITIAL_BUFFER_SIZE);

	if (cryptedMode && bufferSize > 0)
	{
		cfbDecryptor->Put((byte*) &(buffer[0]), bufferSize);
		cfbDecryptor->MessageEnd();

		memcpy(&(buffer[0]),sDecryptOutput.c_str(),bufferSize);
	}
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
buffer_t NetworkSession::readBuffer() throw (NetworkException)
{
	unsigned short len;
	len = readShort();
	if (len + startPosInBuffer > bufferSize)
		throw NetworkException("length + startPosInBuffer > bufferSize");

	char* bufferData = new char[len];
	memcpy(bufferData,&buffer[startPosInBuffer],len);

	startPosInBuffer += len;

	return std::make_pair(bufferData, len);
}

void NetworkSession::SendPacket(NetworkPacket& packet)
{
	if (cryptedMode)
	{
		cfbEncryptor->Put((byte*)&packet.getPacketData()[0], packet.getPacketSize());
	}
	else
	{
		send(socket, &packet.getPacketData()[0], packet.getPacketSize(), 0);
	}
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

}
