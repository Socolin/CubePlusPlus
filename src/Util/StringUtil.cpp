#include "StringUtil.h"

#include <iostream>
namespace Util
{

void StringToWString(std::wstring &ws, const std::string &s)
{
    std::wstring wsTmp(s.begin(), s.end());
    ws = wsTmp;
}

void DumpBuffer(char* buffer, size_t size)
{
    std::cout << "BUFFER DUMP: size:" << size << std::endl;
    std::cout << std::hex;
    int startline = 0;
    for (size_t i = 0; i < size; i++)
    {
        int toPrint = ((int) buffer[i]) & 0xff;
        if (toPrint < 16)
            std::cout << 0;
        std::cout << toPrint << " ";
        if (i % 16 == 15)
        {
            std::cout << " |";
            for (int b = startline; b < startline + 16; b++)
            {
                if (buffer[b] >= 32)
                    std::cout << buffer[b];
                else
                    std::cout << ".";
            }
            std::cout << std::endl;
            startline += 16;
        }
    }
    if (size % 16 != 0)
    {
        int spacecount = (16 - (size % 16)) * 3;
        for (int i = 0; i < spacecount; i++)
            std::cout << " ";
        std::cout << "|";
        for (unsigned b = startline; b < size; b++)
        {
            if (buffer[b] >= 32)
                std::cout << buffer[b];
            else
                std::cout << ".";
        }
    }
    std::cout << std::dec << std::endl;

}

} /* namespace Util */
