#ifndef CHATSTREAM_H_
#define CHATSTREAM_H_

#include <sstream>

#include "Util/StringUtil.h"

namespace Chat
{

enum eColor
{
    BLACK,
    DARK_BLUE,
    DARK_GREEN,
    DARK_AQUA,
    CYAN = DARK_AQUA,
    DARK_RED,
    DARK_PURPLE,
    MAGENTA = DARK_PURPLE,
    GOLD,
    ORANGE = GOLD,
    GRAY,
    DARK_GRAY,
    BLUE,
    GREEN,
    AQUA,
    LIGHT_CYAN = AQUA,
    RED,
    LIGHT_PURPLE,
    PINK = LIGHT_PURPLE,
    YELLOW,
    WHITE,
    OBFUSCATED,
    BOLD,
    STRIKE,
    UNDERLINE,
    ITALIC,
    RESET
};

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

    virtual ChatStream& operator<<(eColor color) = 0;

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
