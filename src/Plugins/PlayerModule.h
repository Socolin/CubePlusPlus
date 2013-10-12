#ifndef PLAYERMODULE_H_
#define PLAYERMODULE_H_

#include <string>
#include "Network/NetworkPacket.h"

namespace World
{
class EntityPlayer;
}
namespace Plugin
{

class PlayerModule
{
public:
    PlayerModule(int moduleId);
    virtual ~PlayerModule();

    int GetModuleId() const;
    virtual const std::string& GetModuleName() = 0;

    virtual void OnPlayerJoinWorld(World::EntityPlayer* player) = 0;
    virtual void OnTickUpdate(World::EntityPlayer* player) = 0;
    virtual void DoAction(World::EntityPlayer* player, int dataType, void* data, void* outData = nullptr) = 0;
    virtual PlayerModule* GetNewModule() = 0;
    virtual void GetCreatePacket(World::EntityPlayer* player, Network::NetworkPacket& packet) = 0;
    virtual void GetUpdatePacket(World::EntityPlayer* player, Network::NetworkPacket& packet) = 0;
private:
    int moduleId;
};

} /* namespace Plugin */
#endif /* PLAYERMODULE_H_ */
