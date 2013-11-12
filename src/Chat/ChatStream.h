#ifndef CHATSTREAM_H_
#define CHATSTREAM_H_

#include <sstream>

#include "Util/StringUtil.h"

namespace Chat
{

class ChatStream
{
public:
    ChatStream();
    virtual ~ChatStream();

    virtual void Send() = 0;

    ChatStream &operator<<(std::ostream & (*/*manip*/)(std::ostream &))
     {
         Send();
         return (*this);
     }

     template<typename T>
     ChatStream& operator<<(T var)
     {
         message << var;
         return (*this);
     }

     ChatStream& operator<<(const std::string& var)
     {
         std::wstring var2;
         Util::StringToWString(var2, var);
         message << var2;
         return (*this);
     }
protected:
    std::wstringstream message;
};

} /* namespace Chat */
#endif /* CHATSTREAM_H_ */
