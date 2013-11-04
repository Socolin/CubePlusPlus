/*
 * Config.cpp
 *
 *  Created on: 28 nov. 2012
 *      Author: poogly
 */

#include "Config.h"
#include "Logging/Logger.h"

#include <string>

namespace Config
{


Config::Config()
    : logDir("log")
    , chunkSentPerTick(16)
    , gamemode(1)
    , fileName("server.cfg")
{
}

Config::~Config()
{
}

void Config::Init()
{
    try
    {
        std::cerr << "Loading Config File " << fileName << std::endl;
        serverConfig.readFile(fileName.c_str());
    }
    catch(const libconfig::FileIOException &fioex)
    {
        std::cerr << "Can't read file " << fileName << ". Aborting." << std::endl;
        exit(EXIT_FAILURE);
    }
    catch(const libconfig::ParseException &pex)
    {
        std::cerr << "Parse error at " << pex.getFile() << ":" << pex.getLine()
            << " - " << pex.getError() << std::endl;
        exit(EXIT_FAILURE);
    }
    serverConfig.lookupValue("server.general.log-dir", logDir);
    if(logDir != "log")
    {
        #ifndef NDEBUG
            Logging::Logger::debug.SetLogDir(logDir + "/debug.log");
        #endif
        Logging::Logger::info.SetLogDir(logDir + "/server.log");
        Logging::Logger::error.SetLogDir(logDir + "/error.log");
    }
    serverConfig.lookupValue("server.world.chunk.chunk-sent-per-tick", chunkSentPerTick);
    if (chunkSentPerTick < 1 || chunkSentPerTick > 16)
    {
        std::cerr << "Invalid chunkSentPerTick value: " <<  chunkSentPerTick << ", value must be between 1 and 16" << std::endl;
        chunkSentPerTick = 16;
    }
    serverConfig.lookupValue("server.general.gamemode", gamemode);
    if(gamemode < 0 || gamemode > 2)
    {
        std::cerr << "Invalid gamemode value: " << gamemode << ", gamemode 1 used (creative)" << std::endl;
        gamemode = 1;
    }
}

void Config::SetConfigFileName(std::string configFileName)
{
    fileName = configFileName;
}

libconfig::Config& Config::GetConfig()
{
    return Config::Instance().serverConfig;
}

unsigned int Config::GetChunkSentPerTick()
{
    return Config::Instance().chunkSentPerTick;
}

int Config::GetGamemode()
{
    return Config::Instance().gamemode;
}

std::string Config::GetLogDir()
{
    return Config::Instance().logDir;
}

} /* namespace Config */
