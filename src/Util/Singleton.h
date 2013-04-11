#ifndef SINGLETON_H_
#define SINGLETON_H_

namespace Util
{

template<typename T> class Singleton
{
public:
    static T& Instance()
    {
        static T instance;
        return instance;
    }
};

} /* namespace Scripting */
#endif /* SINGLETON_H_ */
