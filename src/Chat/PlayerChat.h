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

    virtual ChatStream& operator<<(eColor color)
    {
        switch (color)
        {
        case BLACK:
            message << "§0";
            break;
        case DARK_BLUE:
            message << "§1";
            break;
        case DARK_GREEN:
            message << "§2";
            break;
        case DARK_AQUA:
            message << "§3";
            break;
        case DARK_RED:
            message << "§4";
            break;
        case DARK_PURPLE:
            message << "§5";
            break;
        case GOLD:
            message << "§6";
            break;
        case GRAY:
            message << "§7";
            break;
        case DARK_GRAY:
            message << "§8";
            break;
        case BLUE:
            message << "§9";
            break;
        case GREEN:
            message << "§a";
            break;
        case AQUA:
            message << "§b";
            break;
        case RED:
            message << "§c";
            break;
        case LIGHT_PURPLE:
            message << "§d";
            break;
        case YELLOW:
            message << "§e";
            break;
        case WHITE:
            message << "§f";
            break;
        case OBFUSCATED:
            message << "§k";
            break;
        case BOLD:
            message << "§l";
            break;
        case STRIKE:
            message << "§m";
            break;
        case UNDERLINE:
            message << "§n";
            break;
        case ITALIC:
            message << "§o";
            break;
        case RESET:
            message << "§p";
            break;
        default:
            break;
        }
        return (*this);
    }

    virtual void Send() override;
private:
    World::EntityPlayer* player;
};

} /* namespace Chat */
#endif /* PLAYERCHAT_H_ */
