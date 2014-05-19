#include "PlayerAbilities.h"

#include "EntityPlayer.h"
#include "Network/NetworkPacket.h"

#include <NBTField/TagCompound.h>

namespace World
{

PlayerAbilities::PlayerAbilities(EntityPlayer* player)
    : player(player)
    , disableDamage(false)
    , isFlying(false)
    , allowFlying(false)
    , isCreativeMode(false)
    , allowEdit(true)
    , flySpeed(DEFAULT_FLY_SPEED)
    , walkSpeed(DEFAULT_WALK_SPEED)
    , abilitiesFlag(0)
    , abilityChanged(true)
{
}

PlayerAbilities::~PlayerAbilities()
{
}

void PlayerAbilities::AddAbilities(char capabilities)
{
    abilityChanged = true;
    abilitiesFlag |= capabilities;
    updateValues();
}

void PlayerAbilities::RemoveAbilities(char capabilities)
{
    abilityChanged = true;
    abilitiesFlag &= ~capabilities;
    updateValues();
}

bool PlayerAbilities::hasAbility(ePlayerAbilities capability) const
{
    return abilitiesFlag | capability;
}

void PlayerAbilities::SetWalkSpeed(float value)
{
    abilityChanged = true;
    this->walkSpeed = value;
}

void PlayerAbilities::SetFlySpeed(float value)
{
    abilityChanged = true;
    this->flySpeed = value;
}

float PlayerAbilities::GetWalkSpeed() const
{
    return walkSpeed;
}

float PlayerAbilities::GetFlySpeed() const
{
    return flySpeed;
}

void PlayerAbilities::GetPacket(Network::NetworkPacket& packet)
{
    if (abilityChanged)
    {
        packet << Network::OP_PLAYER_ABILITIES
               << abilitiesFlag
               << static_cast<char>(flySpeed * 255.0f)
               << static_cast<char>(walkSpeed * 255.0f);
        abilityChanged = false;
    }
}

void PlayerAbilities::Update()
{
    if (abilityChanged)
    {
        Network::NetworkPacket packet;
        GetPacket(packet);
        player->Send(packet);
    }
}

void PlayerAbilities::updateFlag()
{
    abilitiesFlag = 0;
    if (isFlying)
    {
        abilitiesFlag |= FLYING;
    }
    if (allowFlying)
    {
        abilitiesFlag |= CAN_FLY;
    }
    if (disableDamage)
    {
        abilitiesFlag |= DAMAGE_DISABLE;
    }
    if (isCreativeMode)
    {
        abilitiesFlag |= CREATIVE_MODE;
    }
}

void PlayerAbilities::updateValues()
{
    isFlying = abilitiesFlag & FLYING;
    allowFlying = abilitiesFlag & CAN_FLY;
    disableDamage = abilitiesFlag & DAMAGE_DISABLE;
    isCreativeMode = abilitiesFlag & CREATIVE_MODE;
}

void PlayerAbilities::Save(NBT::TagCompound* tagAbilities) const
{
    tagAbilities->AddFloat("flySpeed", flySpeed);
    tagAbilities->AddFloat("walkSpeed", walkSpeed);
    tagAbilities->AddByte("flying", isFlying);
    tagAbilities->AddByte("instabuild", isCreativeMode);
    tagAbilities->AddByte("invulnerable", disableDamage);
    tagAbilities->AddByte("mayBuild", allowEdit);
    tagAbilities->AddByte("mayfly", allowFlying);
}

void PlayerAbilities::Load(NBT::TagCompound* tagNbtData)
{
    flySpeed = tagNbtData->GetFloat("flySpeed", DEFAULT_FLY_SPEED);
    walkSpeed = tagNbtData->GetFloat("walkSpeed", DEFAULT_WALK_SPEED);
    isFlying = tagNbtData->GetBool("flying", false);
    isCreativeMode = tagNbtData->GetBool("instabuild", false);
    disableDamage = tagNbtData->GetBool("invulnerable", false);
    allowEdit = tagNbtData->GetBool("mayBuild", true);
    allowFlying = tagNbtData->GetBool("mayfly", false);
    updateFlag();
}

} /* namespace World */
