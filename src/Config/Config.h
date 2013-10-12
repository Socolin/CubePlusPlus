/*
 * Config.h
 *
 *  Created on: 28 nov. 2012
 *      Author: poogly
 */

#ifndef CONFIG_H_
#define CONFIG_H_

#include "Util/Singleton.h"

#include <iostream>
#include <libconfig.h++>

namespace Config
{

class Config : public Util::Singleton<Config>
{
    friend Util::Singleton<Config>;
private:
    Config();
    ~Config();
public:
    static libconfig::Config& getConfig();
    static unsigned int getChunkSentPerTick();
private:
    libconfig::Config serverConfig;
    unsigned int chunkSentPerTick;
};

} /* namespace Config */
#endif /* CONFIG_H_ */
