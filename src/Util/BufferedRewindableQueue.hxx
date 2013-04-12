#include "BufferedRewindableQueue.h"

namespace Util
{

template<typename T, size_t size>
inline Util::BufferedRewindableQueue<T, size>::BufferedRewindableQueue()
{
    countElements = 0;
    currentElement = 0;
}

template<typename T, size_t size>
inline Util::BufferedRewindableQueue<T, size>::~BufferedRewindableQueue()
{
}

template<typename T, size_t size>
inline void Util::BufferedRewindableQueue<T, size>::reset()
{
    countElements = 0;
    currentElement = 0;
}

template<typename T, size_t size>
inline void Util::BufferedRewindableQueue<T, size>::rewind()
{
    currentElement = 0;
}

template<typename T, size_t size>
inline T Util::BufferedRewindableQueue<T, size>::pop()
{
    return datas[currentElement++];
}

template<typename T, size_t size>
inline void Util::BufferedRewindableQueue<T, size>::push(T element)
{
    datas[countElements++] = element;
}

template<typename T, size_t size>
inline bool Util::BufferedRewindableQueue<T, size>::empty()
{
    return currentElement >= countElements;
}

template<typename T, size_t size>
inline bool Util::BufferedRewindableQueue<T, size>::hasSpaceFor(size_t count)
{
    return (size - countElements) > count;
}


} /* namespace Util */
