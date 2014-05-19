#ifndef NETWORKPACKET_H_
#define NETWORKPACKET_H_

#include <iostream>
#include <sstream>
#include <vector>
#include <boost/iostreams/filtering_streambuf.hpp>
#include "OpcodeList.h"

namespace Inventory
{
class ItemStack;
}
namespace Network
{

class NetworkPacket
{
    typedef union
    {
        int i;
        float f;
    } IntToFloat;

    typedef union
    {
        long long l;
        double d;
    } LongToDouble;

public:
    NetworkPacket();
    NetworkPacket(unsigned char opcode);
    NetworkPacket(unsigned char opcode, size_t size);
    virtual ~NetworkPacket();

    // Mettre en inline ?
    void append(const void* data, std::size_t size);

    NetworkPacket& operator <<(bool value);
    NetworkPacket& operator <<(char value);
    NetworkPacket& operator <<(unsigned char value);
    NetworkPacket& operator <<(eOpcode value);
    NetworkPacket& operator <<(short value);
    NetworkPacket& operator <<(unsigned short value);
    NetworkPacket& operator <<(int value);
    NetworkPacket& operator <<(unsigned int value);
    NetworkPacket& operator <<(long long value);
    NetworkPacket& operator <<(float value);
    NetworkPacket& operator <<(double value);
    NetworkPacket& operator <<(const std::wstring& value);
    NetworkPacket& operator <<(const std::pair<char*, short>&);
    NetworkPacket& operator <<(const Inventory::ItemStack* item);
    NetworkPacket& operator <<(const NetworkPacket& packet);

    const std::vector<char>& getPacketData() const
    {
        return packetData;
    }

    size_t getPacketSize() const
    {
        return packetSize;
    }

    void Reset()
    {
        packetSize = 1;
    }

    void Clear()
    {
        packetSize = 0;
    }

    void dump() const;

    void startWriteCompressedData();
    void endWriteCompressedData();
    void appendCompress(char* buffer, size_t size);

    void UpdateAt(int offset, short value);
    void UpdateAt(int offset, int value);
private:
    size_t bufferSize;
    std::vector<char> packetData;
    size_t packetSize;
    int startCompressOffset;
};

} /* namespace Network */
#endif /* NETWORKPACKET_H_ */
