#ifndef AIDROPITEM_H_
#define AIDROPITEM_H_

#include "Util/types.h"

namespace World
{
class LivingEntity;
}
namespace Scripting
{

class LivingEntityScript;
class AIDropItem
{
protected:
    AIDropItem();
    virtual ~AIDropItem();

    void dropItemInit(LivingEntityScript* script, i_item itemId, i_data itemData, i_stackSize itemQuantity, int minTimer, int maxTimer);
    void dropItemUpdate(World::LivingEntity* baseEntity);
private:
    int dropItemTimer;
    int dropItemMinTimer;
    int dropItemMaxTimer;
    i_item dropItemItemId;
    i_damage dropItemItemData;
    i_stackSize dropItemQuantity;

    LivingEntityScript* baseScript;
};

} /* namespace Scripting */
#endif /* AIDROPITEM_H_ */
