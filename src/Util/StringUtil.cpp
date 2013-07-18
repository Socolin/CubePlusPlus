#include "StringUtil.h"

#include <vector>
#include <iostream>
#include <stdexcept>
#include <iconv.h>
#include <boost/locale.hpp>

namespace Util
{

// From http://stackoverflow.com/questions/7153935/how-to-convert-utf-8-stdstring-to-utf-16-stdwstring
std::wstring utf8_to_utf16(const std::string& utf8)
{
    std::vector<unsigned long> unicode;
    size_t i = 0;
    while (i < utf8.size())
    {
        unsigned long uni;
        size_t todo;
        unsigned char ch = utf8[i++];
        if (ch <= 0x7F)
        {
            uni = ch;
            todo = 0;
        }
        else if (ch <= 0xBF)
        {
            throw std::logic_error("not a UTF-8 string");
        }
        else if (ch <= 0xDF)
        {
            uni = ch&0x1F;
            todo = 1;
        }
        else if (ch <= 0xEF)
        {
            uni = ch&0x0F;
            todo = 2;
        }
        else if (ch <= 0xF7)
        {
            uni = ch&0x07;
            todo = 3;
        }
        else
        {
            throw std::logic_error("not a UTF-8 string");
        }
        for (size_t j = 0; j < todo; ++j)
        {
            if (i == utf8.size())
                throw std::logic_error("not a UTF-8 string");
            unsigned char ch = utf8[i++];
            if (ch < 0x80 || ch > 0xBF)
                throw std::logic_error("not a UTF-8 string");
            uni <<= 6;
            uni += ch & 0x3F;
        }
        if (uni >= 0xD800 && uni <= 0xDFFF)
            throw std::logic_error("not a UTF-8 string");
        if (uni > 0x10FFFF)
            throw std::logic_error("not a UTF-8 string");
        unicode.push_back(uni);
    }
    std::wstring utf16;
    for (size_t i = 0; i < unicode.size(); ++i)
    {
        unsigned long uni = unicode[i];
        if (uni <= 0xFFFF)
        {
            utf16 += (wchar_t)uni;
        }
        else
        {
            uni -= 0x10000;
            utf16 += (wchar_t)((uni >> 10) + 0xD800);
            utf16 += (wchar_t)((uni & 0x3FF) + 0xDC00);
        }
    }
    return utf16;
}
void StringToWString(std::wstring &ws, const std::string &s)
{
    try
    {
        ws = utf8_to_utf16(s);
    }
    catch (std::logic_error& e)
    {
        std::wstring wsTmp(s.begin(), s.end());
        ws = wsTmp;
    }
}
void WStringToString(const std::wstring &ws, std::string &s)
{
    s = boost::locale::conv::utf_to_utf<char, wchar_t>(ws);
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
