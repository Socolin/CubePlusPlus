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
            message << L"§0";
            break;
        case DARK_BLUE:
            message << L"§1";
            break;
        case DARK_GREEN:
            message << L"§2";
            break;
        case DARK_AQUA:
            message << L"§3";
            break;
        case DARK_RED:
            message << L"§4";
            break;
        case DARK_PURPLE:
            message << L"§5";
            break;
        case GOLD:
            message << L"§6";
            break;
        case GRAY:
            message << L"§7";
            break;
        case DARK_GRAY:
            message << L"§8";
            break;
        case BLUE:
            message << L"§9";
            break;
        case GREEN:
            message << L"§a";
            break;
        case AQUA:
            message << L"§b";
            break;
        case RED:
            message << L"§c";
            break;
        case LIGHT_PURPLE:
            message << L"§d";
            break;
        case YELLOW:
            message << L"§e";
            break;
        case WHITE:
            message << L"§f";
            break;
        case OBFUSCATED:
            message << L"§k";
            break;
        case BOLD:
            message << L"§l";
            break;
        case STRIKE:
            message << L"§m";
            break;
        case UNDERLINE:
            message << L"§n";
            break;
        case ITALIC:
            message << L"§o";
            break;
        case RESET:
            message << L"§p";
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
