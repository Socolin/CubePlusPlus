/*
 * Config.cpp
 *
 *  Created on: 28 nov. 2012
 *      Author: poogly
 */

#include "Config.h"
#include "Logging/Logger.h"

namespace Config
{


Config::Config()
{
    try
    {
        LOG_INFO << "Loading Config File..." << std::endl;
        serverConfig.readFile("server.cfg");
    }
    catch(const libconfig::FileIOException &fioex)
    {
        LOG_ERROR << "Can't read server.cfg file. Abording." << std::endl;
        exit(EXIT_FAILURE);
    }
    catch(const libconfig::ParseException &pex)
    {
        LOG_ERROR << "Parse error at " << pex.getFile() << ":" << pex.getLine() << " - " << pex.getError() << std::endl;
        exit(EXIT_FAILURE);
    }
}

Config::~Config()
{
}

libconfig::Config& Config::getConfig()
{
    return Config::Instance().serverConfig;
}

} /* namespace Config */
