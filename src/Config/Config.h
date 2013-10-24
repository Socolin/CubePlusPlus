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
#include <string>
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
    static int getGamemode();
    void Init();
    void SetConfigFileName(std::string fileName);
private:
    libconfig::Config serverConfig;
    unsigned int chunkSentPerTick;
    int gamemode;
    std::string fileName;
};

} /* namespace Config */
#endif /* CONFIG_H_ */
