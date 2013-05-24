#ifndef INTUTIL_H_
#define INTUTIL_H_

namespace Util
{
extern int randomGenerator;

inline int FastGenRandomInt()
{
    randomGenerator = randomGenerator * 3 + 1013904223;
    return randomGenerator;
}
}

#endif /* INTUTIL_H_ */
