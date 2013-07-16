#include "NetworkPacket.h"
#include "OpcodeList.h"

namespace Network
{
inline void NetworkSession::SendUpdateTime(long long currentTick, long long ageOfWorld)
{
    NetworkPacket packetUpdateTime(OP_TIME_UPDATE);
    packetUpdateTime << ageOfWorld << currentTick;
    SendPacket(packetUpdateTime);
}

inline void NetworkSession::SendSetExperience(short level, short totalXP, float pct)
{
    NetworkPacket packetUpdateExperience(OP_SET_EXPERIENCE);
    packetUpdateExperience << pct << level << totalXP;
    SendPacket(packetUpdateExperience);
}
inline void NetworkSession::SendUpdateHealth(short health, short food, float foodSaturation)
{
    Network::NetworkPacket packetUpdateHealth(Network::OP_UPDATE_HEALTH);
    packetUpdateHealth << health << food << foodSaturation;
    SendPacket(packetUpdateHealth);
}
inline void NetworkSession::SendSetPositionAndLook(double x, double y, double stance, double z, float yaw, float pitch, bool onGround)
{
    Network::NetworkPacket packetPositionAndLook(Network::OP_PLAYER_POSITION_AND_LOOK);
    packetPositionAndLook << x << y << stance << z << yaw << pitch << onGround;
    SendPacket(packetPositionAndLook);
}
inline void NetworkSession::SendSetAbilities(char walkingSpeed, char flyingSpeed, char abilityFlag)
{
    Network::NetworkPacket packetAbilities(Network::OP_PLAYER_ABILITIES);
    packetAbilities << abilityFlag << walkingSpeed << flyingSpeed;
    SendPacket(packetAbilities);
}
inline void NetworkSession::SendChangeGameState(char reason, char gameMode)
{
    Network::NetworkPacket packetAbilities(Network::OP_CHANGE_GAME_STATE);
    packetAbilities << reason << gameMode;
    SendPacket(packetAbilities);
}
}
