#include "StringUtil.h"

#include <iostream>
namespace Util
{

void StringToWString(std::wstring &ws, const std::string &s)
{
    std::wstring wsTmp(s.begin(), s.end());
    ws = wsTmp;
}
void WStringToString(const std::wstring &ws, std::string &s)
{
    std::string sTmp(ws.begin(), ws.end());
    s = sTmp;
}

void DumpBuffer(char* buffer, size_t size, std::ostream& stream, const char* label)
{
    stream << label << " size:" << size << std::endl;
    stream << std::hex;
    int startline = 0;
    for (size_t i = 0; i < size; i++)
    {
        int toPrint = ((int) buffer[i]) & 0xff;
        if (toPrint < 16)
            stream << 0;
        stream << toPrint << " ";
        if (i % 16 == 15)
        {
            stream << " |";
            for (int b = startline; b < startline + 16; b++)
            {
                if (buffer[b] >= 32)
                    stream << buffer[b];
                else
                    stream << ".";
            }
            stream << std::endl;
            startline += 16;
        }
    }
    if (size % 16 != 0)
    {
        int spacecount = (16 - (size % 16)) * 3;
        for (int i = 0; i < spacecount; i++)
            stream << " ";
        stream << "|";
        for (unsigned b = startline; b < size; b++)
        {
            if (buffer[b] >= 32)
                stream << buffer[b];
            else
                stream << ".";
        }
    }
    stream << std::dec << std::endl;

}

} /* namespace Util */
