#ifndef BUFFEREDREWINDABLEQUEUE_H_
#define BUFFEREDREWINDABLEQUEUE_H_

#include <array>

namespace Util
{

template <typename T, size_t size>
class BufferedRewindableQueue
{
public:
    BufferedRewindableQueue();
    virtual ~BufferedRewindableQueue();

    void reset();
    void rewind();
    T pop();
    void push(T element);
    bool empty();
    bool hasSpaceFor(size_t count);
private:
    int currentElement;
    int countElements;
    std::array<T, size> datas;
};

} /* namespace Util */

#include "Util/BufferedRewindableQueue.hxx"

#endif /* BUFFEREDREWINDABLEQUEUE_H_ */
