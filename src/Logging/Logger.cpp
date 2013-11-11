#include "Logger.h"
#include "Config/Config.h"

#include <sstream>
#include <iomanip>

namespace Logging
{

Logger Logger::debug(std::cout, "debug.log");
Logger Logger::info(std::cout, "server.log");
Logger Logger::error(std::cerr, "error.log");

Logger::Logger(std::ostream& streamOutput, const std::string& fileName)
    : std::ofstream(Config::Config::GetLogDir() + "/" + fileName, std::ios::out | std::ios::app)
    , streamOutput(streamOutput)
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

void Logger::SetLogDir(const std::string& fileName)
{
    close();
    open(fileName.c_str(), std::ios::out | std::ios::app);
}
} /* namespace Logging */

