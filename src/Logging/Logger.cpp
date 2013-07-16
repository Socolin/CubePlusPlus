#include "Logger.h"

#include <sstream>
#include <iomanip>

namespace Logging
{

Logger Logger::debug(std::cout, "log/debug.log");
Logger Logger::info(std::cout, "log/server.log");
Logger Logger::error(std::cerr, "log/error.log");
Logger::Logger(std::ostream& streamOutput, const std::string& fileName)
        : std::ofstream(fileName, std::ios::out | std::ios::app), streamOutput(streamOutput)
{
}

Logger::~Logger()
{
    flush();
    close();
}

std::string getNowDateTime()
{
    std::stringstream date;

    // current date/time based on current system
    time_t now = time(nullptr);
    tm *ltm = localtime(&now);
    date.fill('0');
    date << "[" << std::setw(2) << ltm->tm_mday << "/" << std::setw(2) << 1 + ltm->tm_mon << "/" << 1900 + ltm->tm_year << " " << std::setw(2) << ltm->tm_hour << ":" << std::setw(2) << ltm->tm_min << ":" << std::setw(2) << ltm->tm_sec << "] ";
    return date.str();
}
} /* namespace Logging */

