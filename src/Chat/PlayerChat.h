#ifndef PLAYERCHAT_H_
#define PLAYERCHAT_H_

#include "ChatStream.h"

namespace World
{
class EntityPlayer;
}
namespace Chat
{

class PlayerChat : public ChatStream
{
public:
    PlayerChat(World::EntityPlayer* player);
    virtual ~PlayerChat();

    virtual void Send() override;
private:
    World::EntityPlayer* player;
};

} /* namespace Chat */
#endif /* PLAYERCHAT_H_ */
