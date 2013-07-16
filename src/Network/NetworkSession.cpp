#include "NetworkSession.h"
#include "NetworkException.h"
#include "NetworkExceptionData.h"
#include "NetworkPacket.h"

#include <iostream>
#include <fstream>
#include <unistd.h>
#include <sys/socket.h>

#include "Opcode.h"
#include "World/WorldManager.h"
#include "Entity/EntityPlayer.h"
#include "Util/StringUtil.h"

namespace Network
{
NetworkSession::NetworkSession(int socket) :
    socket(socket), buffer(INITIAL_BUFFER_SIZE),state(STATE_NOTLOGGED),lastKeepAliveTick(0),startPosInBuffer(0)
    ,bufferSize(0),maxBufferSize(0),cryptedMode(false),aesDecryptor(nullptr),aesEncryptor(nullptr),player(nullptr),lastSendKeepAliveTick(0),lastKeepAliveId(0)
    , pendingDataMaxSize(0)
    , pendingDataSize(0)
    , pendingDataPos(0)
{

}

NetworkSession::~NetworkSession()
{
    if (cryptedMode)
    {
        delete aesDecryptor;
        delete aesEncryptor;
    }
    if (player)
    {
        delete player;
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
            maxBufferSize += INITIAL_BUFFER_SIZE;
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
                std::cout << "Receive packet:"<< opcodeTable[packetId].name << " 0x" << std::hex <<  ((int)(packetId)&0xff)  <<std::dec << std::endl;
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

void NetworkSession::disconnect(const char* reason)
{
    if (state == STATE_KICKED || state == STATE_DISCONECT)
        return;
    state = STATE_DISCONECT;
    std::cout << "Disconnect player: "<< reason << std::endl;
    if (player != nullptr)
    {
        player->Disconnect();
        World::WorldManager& worldManager = World::WorldManager::Instance();
        worldManager.RemovePlayer(player);
        player = nullptr;
    }
    close(socket);
}

void NetworkSession::kick(const char* reason)
{
    if (state == STATE_KICKED || state == STATE_DISCONECT)
        return;
    state = STATE_KICKED;

    std::cout << "Kick player: " << reason << std::endl;
    std::wstring wReason;
    std::string sReason(reason);
    Util::StringToWString(wReason, sReason);
    NetworkPacket packet(OP_KICK);
    packet << wReason;
    SendPacket(packet);
    state = STATE_DISCONECT;

    if (player != nullptr)
    {
        World::WorldManager& worldManager = World::WorldManager::Instance();
        worldManager.RemovePlayer(player);
        player->Disconnect();
        player = nullptr;
    }
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
    if (state == STATE_DISCONECT)
        return;

    if (cryptedMode)
    {
        size_t sendSize = 0;
        size_t sendBufferSize = SEND_BUFFER_SIZE;
        size_t toSendSize = packet.getPacketSize();
        size_t sended = 0;
        sendSize = std::min(sendBufferSize,toSendSize);
        while (sendSize > 0)
        {
            aesEncryptor->ProcessData((byte*)sendBuffer,(byte*)&packet.getPacketData()[sended],sendSize);
            SendDelayedData(reinterpret_cast<char*>(sendBuffer), sendSize);
            toSendSize -= sendSize;
            sended += sendSize;
            sendSize = std::min(sendBufferSize,toSendSize);
        }
    }
    else
    {
        send(socket, &packet.getPacketData()[0], packet.getPacketSize(), 0);
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

void NetworkSession::SendDelayedData(char* buffer, int len)
{
    if (HasPendingData())
    {
        while (SendPendingData());
    }

    if (HasPendingData())
    {
        AppendPendingDataToSend(buffer, len);
    }
    else
    {
        struct timeval a;
        a.tv_sec= 0;
        a.tv_usec = 0;
        fd_set rfd;
        FD_ZERO( &rfd );
        FD_SET(socket, &rfd );
        if (select(socket + 1, nullptr, &rfd, nullptr, &a) > 0)
        {
            int res = send(socket, buffer, len, MSG_NOSIGNAL);
            if (res == -1)
            {
                if (errno == EAGAIN)
                {
                    AppendPendingDataToSend(reinterpret_cast<char*>(buffer), len);
                    return;
                }
                perror("send");
                kick("socket error");
                return;
            }
        }
        else
        {
            AppendPendingDataToSend(reinterpret_cast<char*>(buffer), len);
        }
    }
}

void NetworkSession::AppendPendingDataToSend(char* buffer, int len)
{
    if (pendingDataSize + len > pendingDataMaxSize)
    {
        pendingData.resize(pendingDataSize + len);
        pendingDataMaxSize = pendingDataSize + len;
    }

    std::memmove((char*)pendingData.data() + pendingDataSize, buffer, len);
    pendingDataSize += len;
}

bool NetworkSession::HasPendingData()
{
    return pendingDataSize > 0;
}

bool NetworkSession::SendPendingData()
{
    if (state == STATE_DISCONECT)
        return false;

    int pendingSize = pendingDataSize - pendingDataPos;
    if (pendingSize <= 0)
        return false;

    int sendSize = std::min(SEND_BUFFER_SIZE, pendingSize);

    struct timeval a;
    a.tv_sec= 0;
    a.tv_usec = 0;
    fd_set rfd;
    FD_ZERO(&rfd);
    FD_SET(socket, &rfd);
    if (select(socket + 1, nullptr, &rfd, nullptr, &a) > 0)
    {
        int res = send(socket, (char*)pendingData.data() + pendingDataPos, sendSize, MSG_NOSIGNAL);
        if (res == -1)
        {
            if (errno == EAGAIN)
            {
                return false;
            }
            perror("send");
            kick("SendError");
            return false;
        }
        pendingDataPos += res;
    }
    else
    {
        return false;
    }
    if (pendingDataPos >= pendingDataSize)
    {
        pendingDataPos = 0;
        pendingDataSize = 0;
        return false;
    }
    return true;
}

}
