#include "NetworkSession.h"
#include "NetworkException.h"
#include "NetworkExceptionData.h"
#include "NetworkPacket.h"

#include <iostream>
#include <fstream>
#include <unistd.h>
#include <sys/socket.h>

#include "Opcode.h"
#include "Logging/Logger.h"
#include "World/WorldManager.h"
#include "Entity/EntityPlayer.h"
#include "Util/StringUtil.h"

namespace Network
{
NetworkSession::NetworkSession(int socket, const std::string& ip) :
    socket(socket), ip(ip), buffer(INITIAL_BUFFER_SIZE),state(STATE_NOTLOGGED),lastKeepAliveTick(0),startPosInBuffer(0)
    ,bufferSize(0),maxBufferSize(0)
    , cryptedMode(false)
    , aesDecryptor(nullptr)
    , aesDecryptBuffer{0}
    , aesEncryptor(nullptr)
    , aesEncryptBuffer{0}
    , player(nullptr)
    , lastSendKeepAliveTick(0)
    , lastKeepAliveId(0)
    , waitLoginId(0)
{

}

NetworkSession::~NetworkSession()
{
    if (cryptedMode)
    {
        delete aesDecryptor;
        delete aesEncryptor;
        aesDecryptor = nullptr;
        aesEncryptor = nullptr;
        cryptedMode = false;
    }
    if (player)
    {
        player = nullptr;
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
            if (startPosInBuffer > 1024)
            {
                memmove(&(buffer[0]),&(buffer[startPosInBuffer]), bufferSize - startPosInBuffer);
                bufferSize -= startPosInBuffer;
                currentPosInBuffer -= startPosInBuffer;
                startPosInBuffer = 0;
            }
            if (maxBufferSize < bufferSize + INITIAL_BUFFER_SIZE)
            {
                buffer.resize(maxBufferSize + INITIAL_BUFFER_SIZE);
                maxBufferSize += INITIAL_BUFFER_SIZE;
            }
        }
        count = read(socket, &(buffer[bufferSize]), INITIAL_BUFFER_SIZE);
        if (count == -1)
        {
            if (errno == EAGAIN)
                continue;
            throw NetworkException("Read == -1");
        }
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
            if (handler.debug)
                LOG_DEBUG << "Receive packet:"<< opcodeTable[packetId].name << " 0x" << std::hex <<  ((int)(packetId)&0xff)  <<std::dec << std::endl;
            if (bufferSize - startPosInBuffer < handler.packetSize)
            {
                startPosInBuffer = backupStartPos;
                break;
            }
            if ((handler.state & state) != 0)
            {
                (this->*handler.handler) ();
            }
            else
            {
                if (isDisconnected())
                    return;
                else
                    throw NetworkException("Receive bad packet id");
            }
        }
        catch (NetworkExceptionData& e)
        {
            startPosInBuffer = backupStartPos;
            break;
        }
    }
}

void NetworkSession::readData(int length, char* data) throw (NetworkException)
{
    if (length + startPosInBuffer > bufferSize)
        throw NetworkExceptionData("length + startPosInBuffer > bufferSize");

    memcpy(data, &(buffer[startPosInBuffer]), length);
    startPosInBuffer += length;
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

    short result;
    result = *((short*)(&buffer[startPosInBuffer]));
    result = be16toh(result);
    startPosInBuffer += 2;
    return result;
}
float NetworkSession::readFloat() throw (NetworkException)
{
    IntToFloat dbl;
    dbl.i = readInt();
    return dbl.f;
}

void NetworkSession::CloseForDelete()
{
    if (player != nullptr)
    {
        World::WorldManager& worldManager = World::WorldManager::Instance();
        worldManager.RemovePlayer(player);
        player = nullptr;
    }
    close(socket);
}

void NetworkSession::SendKickMessage(const std::wstring& message)
{
    if (isDisconnected())
        return;

    NetworkPacket packet(OP_KICK);
    packet << message;
    SendPacket(packet);
    state = STATE_KICKED;
}

void NetworkSession::disconnect(std::wstring message)
{
    if (isDisconnected())
        return;
    state = STATE_DISCONNECT;

    if (player != nullptr)
    {
        LOG_INFO << L"Disconnect player: " << username << ": " << message << std::endl;
        // Unlink session
        player->Disconnect();
    }
    else
    {
        LOG_INFO << L"Disconnect session: " << message << std::endl;
    }
    shutdown(socket, SHUT_RDWR);
}

void NetworkSession::kick(std::wstring message)
{
    if (isDisconnected())
        return;
    if (player != nullptr)
    {
        LOG_INFO << "Kick " << username << ": " << message << std::endl;
        player->Disconnect();
    }
    else
    {
        LOG_INFO << "Kick sessions : " << message << std::endl;
    }

    SendKickMessage(message);
    state = STATE_KICKED;
    shutdown(socket, SHUT_RDWR);
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

    int result;
    result = *((int*)(&buffer[startPosInBuffer]));
    result = be32toh(result);
    startPosInBuffer += 4;
    return result;
}
long long NetworkSession::readLong() throw (NetworkException)
{
    if (8 + startPosInBuffer > bufferSize)
        throw NetworkExceptionData("8 + startPosInBuffer > bufferSize");

    long long result;
    result = *((long long*)(&buffer[startPosInBuffer]));
    result = be64toh(result);
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

void NetworkSession::SendPacket(const NetworkPacket& packet)
{
    if (isDisconnected())
        return;

    size_t toSendSize = packet.getPacketSize();
    size_t sended = 0;
    while (toSendSize > 0)
    {
        char* data;
        int size = sendBuffer.GetBufferForWriting(&data, toSendSize);
        if (size > 0)
        {
            if (cryptedMode)
            {
                aesEncryptor->ProcessData((byte*)data, (byte*)&packet.getPacketData()[sended], size);
            }
            else
            {
                memcpy(data, &packet.getPacketData()[sended], size);
            }
            toSendSize -= size;
            sended += size;
        }
        else if (size == -1)
        {
            disconnect(L"Buffer is full");
            return;
        }
        else
        {
            break;
        }
    }

    int res = sendBuffer.Send(socket);
    if (res == -1)
    {
        disconnect(L"Error while sending data");
    }
}

std::wstring NetworkSession::readString(const int maxSize) throw (NetworkException)
{
    static wchar_t utf16Text[MAX_STRING_SIZE];
    assert(maxSize < MAX_STRING_SIZE);

    short length = readShort();
    if (length < 0)
        throw NetworkException("String length < 0");

    if (length > maxSize)
        throw NetworkException("String length > maxSize");

    if ((length * 2) + startPosInBuffer >= bufferSize)
        throw NetworkExceptionData("String length < 0");

    for (int i = 0; i < length; i++)
        utf16Text[i] = (short(buffer[startPosInBuffer + (i * 2)]) << 8 & 0xFF00)
                       | (short(buffer[startPosInBuffer + (i * 2 + 1)]) & 0x00FF);
    std::wstring utf16RealText(utf16Text, length);

    startPosInBuffer += length * 2;
    return utf16RealText;
}


bool NetworkSession::IsSendBufferHalfFull()
{
    return sendBuffer.isHalfFull() > 0;
}

}
