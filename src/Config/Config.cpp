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
        fprintf(stdout, "Loading Config File \"%s\" ...\n", fileName.c_str());
        serverConfig.readFile(fileName.c_str());
    }
    catch(const libconfig::FileIOException &fioex)
    {
        fprintf(stderr, "Can't read file \" %s \". Abording.\n", fileName.c_str());
        exit(EXIT_FAILURE);
    }
    catch(const libconfig::ParseException &pex)
    {
        fprintf(stderr, "Parse error at %s : %d - %s\n", pex.getFile(), pex.getLine(), pex.getError());
        exit(EXIT_FAILURE);
    }
    serverConfig.lookupValue("server.general.log-dir", logDir);
    serverConfig.lookupValue("server.world.chunk.chunk-sent-per-tick", chunkSentPerTick);
    if (chunkSentPerTick < 1 || chunkSentPerTick > 16)
    {
        fprintf(stderr, "Invalid chunkSentPerTick value: %d, value must be between 1 and 16\n", chunkSentPerTick);
        chunkSentPerTick = 16;
    }
    serverConfig.lookupValue("server.general.gamemode", gamemode);
    if(gamemode < 0 || gamemode > 2)
    {
        fprintf(stderr, "Invalid gamemode value: %d, gamemode 1 used (survival)\n", gamemode);
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
