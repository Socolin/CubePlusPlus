#ifndef PLAYERABILITIES_H_
#define PLAYERABILITIES_H_

#define DEFAULT_WALK_SPEED 0.1f
#define DEFAULT_FLY_SPEED 0.05f

namespace Network
{
class NetworkPacket;
}
namespace NBT
{
class TagCompound;
}
namespace World
{
class EntityPlayer;

class PlayerAbilities
{
public:
    enum ePlayerAbilities
    {
        DAMAGE_DISABLE = 0X1,
        FLYING = 0X2,
        CAN_FLY = 0X4,
        CREATIVE_MODE = 0X8
    };
public:
    PlayerAbilities(EntityPlayer* player);
    virtual ~PlayerAbilities();

    void AddAbilities(char capabilies);
    void RemoveAbilities(char capabilies);
    bool hasAbility(ePlayerAbilities capability) const;

    void SetWalkSpeed(float value);
    void SetFlySpeed(float vlaue);
    float GetWalkSpeed() const;
    float GetFlySpeed() const;

    void GetPacket(Network::NetworkPacket& packet);
    void Update();

    void Save(NBT::TagCompound* tagNbtData) const;
    void Load(NBT::TagCompound* tagNbtData);
private:
    void updateFlag();
    void updateValues();
private:
    EntityPlayer* player;
    bool disableDamage;
    bool isFlying;
    bool allowFlying;
    bool isCreativeMode;
    bool allowEdit;
    float flySpeed;
    float walkSpeed;
    char abilitiesFlag;
    bool abilityChanged;
};

} /* namespace World */
#endif /* PLAYERABILITIES_H_ */
