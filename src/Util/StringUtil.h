#ifndef STRINGUTIL_H_
#define STRINGUTIL_H_

#include <string>

namespace Util
{

#define UTIL_TEXT_SHELL_BOLD "\033[1m"
#define UTIL_TEXT_SHELL_NONE "\033[0m"

#define UTIL_TEXT_SHELL_BOLD_BLACK      "\e[1;30m"
#define UTIL_TEXT_SHELL_BOLD_RED        "\e[1;31m"
#define UTIL_TEXT_SHELL_BOLD_GREEN      "\e[1;32m"
#define UTIL_TEXT_SHELL_BOLD_YELLOW     "\e[1;33m"
#define UTIL_TEXT_SHELL_BOLD_BLUE       "\e[1;34m"
#define UTIL_TEXT_SHELL_BOLD_PURPLE     "\e[1;35m"
#define UTIL_TEXT_SHELL_BOLD_CYAN       "\e[1;36m"
#define UTIL_TEXT_SHELL_BOLD_WHITE      "\e[1;37m"

void StringToWString(std::wstring &ws, const std::string &s);


} /* namespace Util */
#endif /* STRINGUTIL_H_ */
