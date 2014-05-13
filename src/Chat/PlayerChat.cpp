#include "PlayerChat.h"

#include "Network/NetworkPacket.h"
#include "Network/OpcodeList.h"
#include "Entity/EntityPlayer.h"

namespace Chat
{

PlayerChat::PlayerChat(World::EntityPlayer* player)
    : player(player)
    , isMuted(false)
{

}

PlayerChat::~PlayerChat()
{
}

void PlayerChat::Send()
{
    size_t sended = 0;
    std::wstring lines = message.str();
    while (sended < lines.size())
    {
        Network::NetworkPacket packet(Network::OP_CHAT_MESSAGE);
        const std::wstring &line = lines.substr(sended, 127 + sended);
        packet << line;
        sended += line.size();
        player->Send(packet);
    }
    message.str(std::wstring());
}

void PlayerChat::Mute(bool mute)
{
    isMuted = mute;
}

bool PlayerChat::IsMuted()
{
    return isMuted;
}

} /* namespace Chat */
