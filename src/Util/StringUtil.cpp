#include "StringUtil.h"

namespace Util
{

void StringToWString(std::wstring &ws, const std::string &s)
{
    std::wstring wsTmp(s.begin(), s.end());
    ws = wsTmp;
}


} /* namespace Util */
