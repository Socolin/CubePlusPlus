#ifndef ITEMSCRIPT_H_
#define ITEMSCRIPT_H_

#include <string>
#include "Util/types.h"

namespace Inventory
{
class Item;
class ItemStack;
}
namespace World
{
class World;
class Entity;
class EntityPlayer;
}
namespace Scripting
{
class ItemScript
{
public:
    ItemScript(const char* scriptName);
    virtual ~ItemScript();

    void Init(Inventory::Item* item);

    virtual void InitParam(int /*paramId*/, int /*param*/) {}
    virtual void InitParam(int /*paramId*/, float /*param*/) {}
    virtual void InitParam(int /*paramId*/, const std::string& /*param*/) {}

    // Return false if nothing done, return true if item has been used
    virtual ItemUseResult OnUse(World::EntityPlayer* user) const;
    virtual ItemUseResult OnUseOnEntity(World::EntityPlayer* user, World::Entity* target) const;
    virtual ItemUseResult OnUseOnBlock(World::EntityPlayer* user, int x, unsigned char y, int z, char face, char cursorPositionX, char cursorPositionY, char cursorPositionZ) const;

    virtual ItemScript* Copy() = 0;
protected:
    Inventory::Item* baseItem;
};

} /* namespace Scripting */
#endif /* ITEMSCRIPT_H_ */
