#ifndef LOGGER_H_
#define LOGGER_H_

#include <fstream>
#include <iostream>
#include <ctime>

#include "Util/StringUtil.h"

#ifndef NDEBUG
# define LOG_DEBUG Logging::Logger::debug << Logging::getNowDateTime()
#else
# define LOG_DEBUG Logging::FalseLogger::debug
#endif

#define LOG_INFO Logging::Logger::info << Logging::getNowDateTime()
#define LOG_INFO_CONCAT Logging::Logger::info
#define LOG_ERROR Logging::Logger::error << Logging::getNowDateTime()

namespace Logging
{
std::string getNowDateTime();
class Logger: public std::ofstream
{
public:
    static Logger debug;
    static Logger info;
    static Logger error;
public:
    Logger(std::ostream& streamOutput, const std::string& fileName);
    virtual ~Logger();

    Logger &operator<<(std::ios_base & (*manip)(std::ostream &))
    {
        manip(streamOutput);
        manip(static_cast<std::ofstream&>(*this));
        return (*this);
    }

    Logger &operator<<(std::ostream & (*manip)(std::ostream &))
    {
        manip(streamOutput);
        manip(static_cast<std::ofstream&>(*this));
        return (*this);
    }

    template<typename T>
    Logger& operator<<(T var)
    {
        streamOutput << var;
        static_cast<std::ofstream&>(*this) << var;
        return (*this);
    }

    Logger& operator<<(const std::wstring& var)
    {
        std::string var2;
        Util::WStringToString(var, var2);
        streamOutput << var2;
        static_cast<std::ofstream&>(*this) << var2;
        return (*this);
    }

    Logger& operator<<(const wchar_t* var)
    {
        std::wstring str(var);
        return *this << str;
    }
private:
    std::ostream& streamOutput;
};

class FalseLogger: public std::ofstream
{
public:
    static FalseLogger debug;
    static FalseLogger info;
    static FalseLogger error;
public:

    FalseLogger &operator<<(std::ostream & (*/*manip*/)(std::ostream &))
    {
        return (*this);
    }
    template<typename T>
    FalseLogger& operator<<(const T& /*var*/)
    {
        return *this;
    }
    FalseLogger& operator<<(const std::wstring& /*var*/)
    {
        return (*this);
    }
};

} /* namespace Logging */
#endif /* LOGGER_H_ */
