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
    virtual ~Singleton()
    {

    }
    virtual void InitInstance()
    {

    }
protected:
    Singleton()
    {
    }
};

} /* namespace Util */
#endif /* SINGLETON_H_ */
