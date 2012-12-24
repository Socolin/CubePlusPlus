#include "NetworkSession.h"
#include "NetworkException.h"
#include "NetworkExceptionData.h"
#include "NetworkPacket.h"

#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include "Opcode.h"

namespace Network
{
NetworkSession::NetworkSession(int socket) :
		socket(socket), buffer(INITIAL_BUFFER_SIZE),state(STATE_NOTLOGGED),lastKeepAliveTick(0),startPosInBuffer(0),bufferSize(0),maxBufferSize(0),cryptedMode(false),aesDecryptor(NULL),aesEncryptor(NULL)
{

}

NetworkSession::~NetworkSession()
{
	if (cryptedMode)
	{
		delete aesDecryptor;
		delete aesEncryptor;
	}
}

void NetworkSession::ReceiveInBuffer() throw (NetworkException)
{
	int count = 0;
	if (startPosInBuffer == bufferSize)
	{
		startPosInBuffer = 0;
		bufferSize = 0;
	}
	int currentPosInBuffer = bufferSize;
	do
	{
		if (maxBufferSize < bufferSize + INITIAL_BUFFER_SIZE)
		{
			buffer.resize(maxBufferSize + INITIAL_BUFFER_SIZE);
		}
		count = read(socket, &(buffer[bufferSize]), INITIAL_BUFFER_SIZE);
		if (count == -1)
		{
			if (errno == EAGAIN)
				continue;
			throw NetworkException("Read == -1");
		}
		//std::cout << "read:" << count << std::endl;
		bufferSize += count;
	}
	while (count == INITIAL_BUFFER_SIZE);

	if (cryptedMode && bufferSize - currentPosInBuffer > 0)
	{
		aesDecryptor->ProcessData((byte*) &(buffer[currentPosInBuffer]),(byte*) &(buffer[currentPosInBuffer]), bufferSize - currentPosInBuffer);
	}
}
void NetworkSession::ReceiveData() throw (NetworkException)
{
	ReceiveInBuffer();
	while (startPosInBuffer < bufferSize)
	{
		uint32_t backupStartPos = startPosInBuffer;
		try
		{
			unsigned char packetId = readByte();
			const OpcodeHandler& handler = opcodeTable[packetId];
			std::cout << "Receive packet:"<< opcodeTable[packetId].name << " 0x" << std::hex <<  ((int)(packetId)&0xff)  <<std::dec << std::endl;
			if (handler.state == STATE_NEVER)
			{
				throw NetworkException("Receive bad packet id");
			}
			(this->*handler.handler) ();
		}
		catch (NetworkExceptionData& e)
		{
			startPosInBuffer = backupStartPos;
			break;
		}
		std::cout << "packetSize: " << (startPosInBuffer - backupStartPos) << std::endl;
	}
}

char NetworkSession::readByte() throw (NetworkException)
{
	if (1 + startPosInBuffer > bufferSize)
		throw NetworkExceptionData("1 + startPosInBuffer > bufferSize");

	return buffer[startPosInBuffer++];
}

short NetworkSession::readShort() throw (NetworkException)
{
	if (2 + startPosInBuffer > bufferSize)
		throw NetworkExceptionData("2 + startPosInBuffer > bufferSize");
	short result = 0;
	result =  (short(buffer[startPosInBuffer])<<8 & 0xFF00) |
			(short(buffer[startPosInBuffer + 1]) & 0x00FF);
	startPosInBuffer += 2;
	return result;
}
float NetworkSession::readFloat() throw (NetworkException)
{
	IntToFloat dbl;
	dbl.i = readInt();
	return dbl.f;
}
double NetworkSession::readDouble() throw (NetworkException)
{
	LongToDouble dbl;
	dbl.i = readLong();
	return dbl.f;
}
int NetworkSession::readInt() throw (NetworkException)
{
	if (4 + startPosInBuffer > bufferSize)
		throw NetworkExceptionData("4 + startPosInBuffer > bufferSize");

	int result = 0;
	result = buffer[startPosInBuffer] << 24;
	result += buffer[startPosInBuffer + 1] << 16;
	result += buffer[startPosInBuffer + 2] << 8;
	result += buffer[startPosInBuffer + 3];
	startPosInBuffer += 4;
	return result;
}
long NetworkSession::readLong() throw (NetworkException)
{
	if (8 + startPosInBuffer > bufferSize)
		throw NetworkExceptionData("8 + startPosInBuffer > bufferSize");

	long result = 0;
	result = ((long)buffer[startPosInBuffer]) << 56;
	result += ((long)buffer[startPosInBuffer + 1]) << 48;
	result += ((long)buffer[startPosInBuffer + 2]) << 40;
	result += ((long)buffer[startPosInBuffer + 3]) << 32;
	result += buffer[startPosInBuffer + 4] << 24;
	result += buffer[startPosInBuffer + 5] << 16;
	result += buffer[startPosInBuffer + 6] << 8;
	result += buffer[startPosInBuffer + 7];
	startPosInBuffer += 8;
	return result;
}
buffer_t NetworkSession::readBuffer() throw (NetworkException)
{
	unsigned short len;
	len = readShort();
	if (len + startPosInBuffer > bufferSize)
		throw NetworkExceptionData("length + startPosInBuffer > bufferSize");

	char* bufferData = new char[len];
	memcpy(bufferData,&buffer[startPosInBuffer],len);

	startPosInBuffer += len;

	return std::make_pair(bufferData, len);
}

void NetworkSession::SendPacket(NetworkPacket& packet)
{
	if (cryptedMode)
	{
		aesEncryptor->ProcessData((byte*)&packet.getPacketData()[0],(byte*)&packet.getPacketData()[0],packet.getPacketSize());
	}
	send(socket, &packet.getPacketData()[0], packet.getPacketSize(), 0);
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
