/*
 * NetworkPacket.cpp
 *
 *  Created on: 22 nov. 2012
 *      Author: bertrand
 */

#include "NetworkPacket.h"

#include <zlib.h>
#include <cstring>


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
    tmpBuffer[0] = (value >> 8) & 0xff;
    tmpBuffer[1] = value & 0xff;
    append(tmpBuffer, 2);
    return *this;
}
NetworkPacket& NetworkPacket::operator <<(unsigned short value)
{
    tmpBuffer[0] = (value >> 8) & 0xff;
    tmpBuffer[1] = value & 0xff;
    append(tmpBuffer, 2);
    return *this;
}

NetworkPacket& NetworkPacket::operator <<(int value)
{
    tmpBuffer[0] = (value >> 24) & 0xff;
    tmpBuffer[1] = (value >> 16) & 0xff;
    tmpBuffer[2] = (value >> 8) & 0xff;
    tmpBuffer[3] = value & 0xff;
    append(tmpBuffer, 4);
    return *this;
}

NetworkPacket& NetworkPacket::operator <<(long value)
{
    tmpBuffer[0] = (value >> 56) & 0xff;
    tmpBuffer[1] = (value >> 48) & 0xff;
    tmpBuffer[2] = (value >> 40) & 0xff;
    tmpBuffer[3] = (value >> 32) & 0xff;
    tmpBuffer[4] = (value >> 24) & 0xff;
    tmpBuffer[5] = (value >> 16) & 0xff;
    tmpBuffer[6] = (value >> 8) & 0xff;
    tmpBuffer[7] = value & 0xff;
    append(tmpBuffer, 8);
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

NetworkPacket& NetworkPacket::operator <<(const std::pair<char*, short>& pair)
{
    short value = pair.second;
    tmpBuffer[0] = (value >> 8) & 0xff;
    tmpBuffer[1] = value & 0xff;
    append(tmpBuffer, 2);
    append(pair.first, pair.second);
    return *this;
}

void NetworkPacket::dump() const
{
    std::cout << "PACKET DUMP: size:" << packetSize << std::endl;
    std::cout << std::hex;
    int startline = 0;
    for (size_t i = 0; i < packetSize; i++)
    {
        int toPrint = ((int) packetData[i]) & 0xff;
        if (toPrint <= 16)
            std::cout << 0;
        std::cout << toPrint << " ";
        if (i % 16 == 15)
        {
            std::cout << " |";
            for (int b = startline; b < startline + 16; b++)
            {
                if (packetData[b] >= 32 && packetData[b] <= 127)
                    std::cout << packetData[b];
                else
                    std::cout << ".";
            }
            std::cout << std::endl;
            startline += 16;
        }
    }
    if (packetSize % 16 != 0)
    {
        int spacecount = (16 - (packetSize % 16)) * 3;
        for (int i = 0; i < spacecount; i++)
            std::cout << " ";
        std::cout << "|";
        for (unsigned b = startline; b < packetSize; b++)
        {
            if (packetData[b] >= 32 && packetData[b] <= 127)
                std::cout << packetData[b];
            else
                std::cout << ".";
        }
    }
    std::cout << std::dec << std::endl;
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

       stream.zalloc = (alloc_func)0;
       stream.zfree = (free_func)0;
       stream.opaque = (voidpf)0;


       err = deflateInit2(&stream, Z_DEFAULT_COMPRESSION, Z_DEFLATED, 15, 8, Z_DEFAULT_STRATEGY);
       if (err != Z_OK) return err;

       err = deflate(&stream, Z_FINISH);
       if (err != Z_STREAM_END) {
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

} /* namespace Network */
