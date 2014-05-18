#ifndef CHATCOMMANDMANAGER_HXX_
#define CHATCOMMANDMANAGER_HXX_

#include "Util/StringUtil.h"

namespace Chat
{

template<typename T>
ChatCommand* createChatCommand(const CommandSender& sender, const std::vector<std::string>& splitedCommand)
{
    return new T(sender, splitedCommand);
}

inline static void addInSplitedCommand(const std::string& str, std::vector<std::string>& outSplitedCommand)
{
    outSplitedCommand.push_back(str);
}

inline static void addInSplitedCommand(const std::wstring& str, std::vector<std::string>& outSplitedCommand)
{
    std::string strUtf8;
    Util::WStringToString(str, strUtf8);
    outSplitedCommand.push_back(strUtf8);
}

template<typename CharType>
bool ChatCommandManager::parseMessage(std::vector<std::string>& outSplitedCommand, const std::basic_string<CharType>& message) const
{
    size_t messageLen = message.size();
    std::basic_string<CharType> splitElement;
    bool inQuote = false;
    const CharType SPACE = ' ';
    const CharType DOUBLE_QUOTE = '"';
    const CharType BACKSLASH = '\\';
    CharType previous = '/';
    // Ignore first character, it will always be '/'
    for (size_t i = 1; i < messageLen; i++)
    {
        CharType c = message[i];

        if (inQuote) {
            if (c == DOUBLE_QUOTE && previous != BACKSLASH)
            {
                inQuote = false;
            }
            else if (c == BACKSLASH && previous != BACKSLASH)
            {

            }
            else
            {
                splitElement += c;
            }
        } else {
            if (c == SPACE)
            {
                addInSplitedCommand(splitElement, outSplitedCommand);
                splitElement.clear();
            }
            else if (c == DOUBLE_QUOTE && previous != BACKSLASH)
            {
                inQuote = true;
            }
            else if (c == BACKSLASH && previous != BACKSLASH)
            {

            }
            else
            {
                splitElement += c;
            }
        }
        previous = c;
    }
    if (inQuote)
    {
        return false;
    }
    addInSplitedCommand(splitElement, outSplitedCommand);
    splitElement.clear();
    return true;
}

}
#endif /* CHATCOMMANDMANAGER_HXX_ */
