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
        LOG_INFO << "Loading Config File..." << std::endl;
        serverConfig.readFile(fileName.c_str());
    }
    catch(const libconfig::FileIOException &fioex)
    {
        LOG_ERROR << "Can't read file \"" << fileName << "\". Abording." << std::endl;
        exit(EXIT_FAILURE);
    }
    catch(const libconfig::ParseException &pex)
    {
        LOG_ERROR << "Parse error at " << pex.getFile() << ":" << pex.getLine() << " - " << pex.getError() << std::endl;
        exit(EXIT_FAILURE);
    }
    serverConfig.lookupValue("server.general.log-dir", logDir);
    serverConfig.lookupValue("server.world.chunk.chunk-sent-per-tick", chunkSentPerTick);
    if (chunkSentPerTick < 1 || chunkSentPerTick > 16)
    {
        LOG_ERROR << "Invalid chunkSentPerTick value: " << chunkSentPerTick << ", value must be between 1 and 16" << std::endl;
        chunkSentPerTick = 16;
    }
    serverConfig.lookupValue("server.general.gamemode", gamemode);
    if(gamemode < 0 || gamemode > 2)
    {
        LOG_ERROR << "Invalid gamemode value: " << gamemode << ", gamemode 1 used (survival)" << std::endl;
        gamemode = 1;
    }
}

void Config::SetConfigFileName(std::string configFileName)
{
    fileName = configFileName;
}

libconfig::Config& Config::getConfig()
{
    return Config::Instance().serverConfig;
}

unsigned int Config::getChunkSentPerTick()
{
    return Config::Instance().chunkSentPerTick;
}

int Config::getGamemode()
{
    return Config::Instance().gamemode;
}

std::string Config::getLogDir()
{
    return Config::Instance().logDir;
}

} /* namespace Config */
