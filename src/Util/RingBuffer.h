#ifndef RINGBUFFER_H_
#define RINGBUFFER_H_

#include <array>
#include <cerrno>
#include <cstdio>
#include <sys/socket.h>

namespace Util
{

template <size_t BufferSizeNPow>
class RingBuffer
{
public:
    RingBuffer();
    virtual ~RingBuffer();

    int Write(size_t len, char* data);
    int Read(size_t len, char* data);
    int Send(int socket);

    size_t GetBufferForWriting(char** buffer, size_t len);

    bool isFull(int len = 0);
    bool isHalfFull()
    {
        return (writePos - readPos) >= (1 << (BufferSizeNPow - 1));
    }
private:
    const size_t size = (1 << BufferSizeNPow);
    const size_t mask = size - 1;
    std::array<char, 1 << BufferSizeNPow> buffer;
    size_t readPos;
    size_t writePos;
};

template<size_t BufferSizeNPow>
inline RingBuffer<BufferSizeNPow>::RingBuffer()
    : readPos(0), writePos(0)
{
}

template<size_t BufferSizeNPow>
inline RingBuffer<BufferSizeNPow>::~RingBuffer()
{
}

template<size_t BufferSizeNPow>
inline int RingBuffer<BufferSizeNPow>::Write(size_t len, char* data)
{
    if (isFull(len))
        return 0;
    int firstWrite = std::min((size - (writePos & mask)), len);
    memcpy(&(buffer[writePos]), data, firstWrite);
    writePos += firstWrite;

    int secondWrite = len - firstWrite;
    if (secondWrite > 0)
    {
        memcpy(&(buffer[writePos & mask]), data + firstWrite, secondWrite);
        writePos += secondWrite;
    }
    else if (writePos > size && readPos > size)
    {
        writePos = writePos & mask;
        readPos = readPos & mask;
    }
    return len;
}

template<size_t BufferSizeNPow>
inline bool Util::RingBuffer<BufferSizeNPow>::isFull(int len)
{
    return (writePos - readPos) + len >= size;
}

template<size_t BufferSizeNPow>
inline size_t RingBuffer<BufferSizeNPow>::GetBufferForWriting(char** outBuffer, size_t len)
{
    if (isFull(len))
        return -1;
    int writeLen = std::min((size - (writePos & mask)), len);

    *outBuffer = &(buffer[writePos & mask]);
    writePos += writeLen;
    return writeLen;
}

template<size_t BufferSizeNPow>
inline int RingBuffer<BufferSizeNPow>::Read(size_t len, char* data)
{
    if (readPos == writePos)
        return 0;

    size_t avaible = writePos - readPos;

    size_t toRead = std::min(size - (readPos & mask), avaible);
    toRead = std::min(toRead, len);

    memcpy(data, &(buffer[readPos & mask]), toRead);
    readPos += toRead;

    if (toRead < len)
    {
        toRead += Read(len - toRead, data + toRead);
    }
    return toRead;
}

template<size_t BufferSizeNPow>
inline int Util::RingBuffer<BufferSizeNPow>::Send(int socket)
{
    if (readPos == writePos)
        return 0;

    int count = 0;
    int res = 0;

    size_t avaible = writePos - readPos;

    do
    {
        size_t toRead = std::min(size - (readPos & mask), avaible);
        res = send(socket, &(buffer[readPos & mask]), toRead, MSG_NOSIGNAL);
        if (res == -1)
        {
            if (errno == EAGAIN || errno == EWOULDBLOCK)
                break;
            perror("send");
            return -1;
        }
        avaible -= res;
        readPos += res;
        count += res;
    } while (avaible > 0);

    return count;
}

} /* namespace Util */


#endif /* RINGBUFFER_H_ */
