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

static bool instanced = false;
static libconfig::Config ServerConfig;

Config::Config()
{
    try
    {
        LOG_INFO << "Loading Config File..." << std::endl;
        ServerConfig.readFile("server.cfg");
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

    instanced = true;
}

Config::~Config()
{
    //ServerConfig.~Config();
}

libconfig::Config& Config::getConfig()
{
    if(instanced == false)
    {
        Config();
    }

    return ServerConfig;
}

} /* namespace Config */
