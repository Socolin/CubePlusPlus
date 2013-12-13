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
    static libconfig::Config& GetConfig();
    static unsigned int GetChunkSentPerTick();
    static int GetGamemode();
    static std::string GetLogDir();
    void Init();
    void SetConfigFileName(std::string fileName);
private:
    libconfig::Config serverConfig;
    std::string logDir;
    unsigned int chunkSentPerTick;
    int gamemode;
    std::string fileName;
};

} /* namespace Config */
#endif /* CONFIG_H_ */
