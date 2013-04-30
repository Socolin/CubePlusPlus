#ifndef STRINGUTIL_H_
#define STRINGUTIL_H_

#include <string>

namespace Util
{

#define UTIL_TEXT_SHELL_BOLD "\033[1m"
#define UTIL_TEXT_SHELL_NONE "\033[0m"

#define UTIL_TEXT_SHELL_BOLD_BLACK      "\033[1;30m"
#define UTIL_TEXT_SHELL_BOLD_RED        "\033[1;31m"
#define UTIL_TEXT_SHELL_BOLD_GREEN      "\033[1;32m"
#define UTIL_TEXT_SHELL_BOLD_YELLOW     "\033[1;33m"
#define UTIL_TEXT_SHELL_BOLD_BLUE       "\033[1;34m"
#define UTIL_TEXT_SHELL_BOLD_PURPLE     "\033[1;35m"
#define UTIL_TEXT_SHELL_BOLD_CYAN       "\033[1;36m"
#define UTIL_TEXT_SHELL_BOLD_WHITE      "\033[1;37m"

void StringToWString(std::wstring &ws, const std::string &s);


} /* namespace Util */
#endif /* STRINGUTIL_H_ */
