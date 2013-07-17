/*
 * NetworkPacket.cpp
 *
 *  Created on: 22 nov. 2012
 *      Author: bertrand
 */

#include "NetworkPacket.h"

#include <zlib.h>
#include <cstring>
#include <cppnbt.h>

#include "Inventory/ItemStack.h"
#include "Logging/Logger.h"

namespace Network
{

NetworkPacket::NetworkPacket() :
    bufferSize(32), packetData(32), packetSize(0), startCompressOffset(0)
{

}
NetworkPacket::NetworkPacket(unsigned char opcode) :
    bufferSize(32), packetData(32), packetSize(0), startCompressOffset(0)
{
    append(&opcode, 1);
}
NetworkPacket::NetworkPacket(unsigned char opcode, size_t minSize) :
    bufferSize(minSize), packetData(minSize), packetSize(0), startCompressOffset(0)
{
    append(&opcode, 1);
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

NetworkPacket& NetworkPacket::operator <<(bool value)
{
    append(&value, 1);
    return *this;
}
NetworkPacket& NetworkPacket::operator <<(char value)
{
    append(&value, 1);
    return *this;
}
NetworkPacket& NetworkPacket::operator <<(unsigned char value)
{
    append(&value, 1);
    return *this;
}

NetworkPacket& NetworkPacket::operator <<(short value)
{
    value = htobe16(value);
    append(&value, 2);
    return *this;
}
NetworkPacket& NetworkPacket::operator <<(unsigned short value)
{
    value = htobe16(value);
    append(&value, 2);
    return *this;
}

NetworkPacket& NetworkPacket::operator <<(int value)
{
    value = htobe32(value);
    append(&value, 4);
    return *this;
}

NetworkPacket& NetworkPacket::operator <<(unsigned int value)
{
    value = htobe32(value);
    append(&value, 4);
    return *this;
}

NetworkPacket& NetworkPacket::operator <<(long long value)
{
    value = htobe64(value);
    append(&value, 8);
    return *this;
}

NetworkPacket& NetworkPacket::operator <<(const Inventory::ItemStack* item)
{
    if (item == nullptr)
    {
        *this << short(-1);
    }
    else
    {
        *this << item->getItemId();
        if (item->getItemId() > 0)
        {
            *this << (char)item->getStackSize() << item->getItemData();
            nbt::Tag* specialData = item->GetSpecialData();
            if (specialData != nullptr)
            {
                nbt::NbtBuffer nbtBuffer;
                unsigned int len = 0;

                char* buffer = nbtBuffer.writeGzip(specialData, len);
                if (buffer != nullptr)
                {
                    *this << (short)len;
                    append(buffer, len);
                }
                else
                {
                    *this << short(-1) /*No nbt data*/;
                }
            }
            else
            {
                *this << short(-1) /*No nbt data*/;
            }
        }
    }
    return *this;
}

NetworkPacket& NetworkPacket::operator <<(float value)
{
    IntToFloat conv;
    conv.f = value;
    return (*this << conv.i);
}

NetworkPacket& NetworkPacket::operator <<(double value)
{
    LongToDouble conv;
    conv.d = value;
    return (*this << conv.l);
}

NetworkPacket& NetworkPacket::operator <<(const std::wstring& value)
{
    short len = value.size();
    short strLenght = htobe16(len);
    append(&strLenght, 2);
    const wchar_t* datastr = value.c_str();
    for (short i = 0; i < len; i++)
    {
        short val = datastr[i];
        val = htobe16(val);
        append(&val, 2);
    }
    return *this;
}

NetworkPacket& NetworkPacket::operator <<(const std::pair<char*, short>& pair)
{
    short value = pair.second;
    value = htobe16(value);
    append(&value, 2);

    append(pair.first, pair.second);
    return *this;
}

NetworkPacket& NetworkPacket::operator <<(const NetworkPacket& packet)
{
    append(packet.packetData.data(), packet.packetSize);
    return *this;
}

void NetworkPacket::dump() const
{
    LOG_DEBUG << "PACKET DUMP: size:" << packetSize << std::endl;
    LOG_DEBUG << std::hex;
    int startline = 0;
    for (size_t i = 0; i < packetSize; i++)
    {
        int toPrint = ((int) packetData[i]) & 0xff;
        if (toPrint <= 16)
            LOG_DEBUG << 0;
        LOG_DEBUG << toPrint << " ";
        if (i % 16 == 15)
        {
            LOG_DEBUG << " |";
            for (int b = startline; b < startline + 16; b++)
            {
                if (packetData[b] >= 32)
                    LOG_DEBUG << packetData[b];
                else
                    LOG_DEBUG << ".";
            }
            LOG_DEBUG << std::endl;
            startline += 16;
        }
    }
    if (packetSize % 16 != 0)
    {
        int spacecount = (16 - (packetSize % 16)) * 3;
        for (int i = 0; i < spacecount; i++)
            LOG_DEBUG << " ";
        LOG_DEBUG << "|";
        for (unsigned b = startline; b < packetSize; b++)
        {
            if (packetData[b] >= 32)
                LOG_DEBUG << packetData[b];
            else
                LOG_DEBUG << ".";
        }
    }
    LOG_DEBUG << std::dec << std::endl;
}

void NetworkPacket::startWriteCompressedData()
{
    startCompressOffset = packetSize;
    packetSize += 8;
}
//err = deflateInit2(&stream, level, Z_DEFLATED, -15, 8, Z_DEFAULT_STRATEGY);
int my_compress (Bytef *dest,   uLongf *destLen,
                 const Bytef *source, uLong sourceLen)
{
    z_stream stream;
    int err;

    stream.next_in = (Bytef*)source;
    stream.avail_in = (uInt)sourceLen;
#ifdef MAXSEG_64K
    /* Check for source > 64K on 16-bit machine: */
    if ((uLong)stream.avail_in != sourceLen) return Z_BUF_ERROR;
#endif
    stream.next_out = dest;
    stream.avail_out = (uInt)*destLen;
    if ((uLong)stream.avail_out != *destLen) return Z_BUF_ERROR;

    stream.zalloc = (alloc_func)nullptr;
    stream.zfree = (free_func)nullptr;
    stream.opaque = (voidpf)nullptr;


    err = deflateInit2(&stream, Z_DEFAULT_COMPRESSION, Z_DEFLATED, 15, 8, Z_DEFAULT_STRATEGY);
    if (err != Z_OK) return err;

    err = deflate(&stream, Z_FINISH);
    if (err != Z_STREAM_END)
    {
        deflateEnd(&stream);
        return err == Z_OK ? Z_BUF_ERROR : err;
    }
    *destLen = stream.total_out;

    err = deflateEnd(&stream);
    return err;
}

void NetworkPacket::endWriteCompressedData()
{
    uLongf newSize = packetSize - startCompressOffset;
    compress2((Bytef*)&packetData[startCompressOffset + 4],&newSize, (Bytef*)&packetData[startCompressOffset + 8], packetSize - startCompressOffset - 8, Z_DEFAULT_COMPRESSION);
    packetSize = startCompressOffset;
    *this << (int)newSize;
    packetSize = startCompressOffset + newSize + 4;
}

void NetworkPacket::appendCompress(char* buffer, size_t size)
{
    append(buffer, size);
}

void NetworkPacket::UpdateAt(int offset, int value)
{
    value = htobe32(value);
    std::memcpy(&packetData[offset], &value, 4);
}

void NetworkPacket::UpdateAt(int offset, short value)
{
    value = htobe16(value);
    std::memcpy(&packetData[offset], &value, 2);
}

} /* namespace Network */
