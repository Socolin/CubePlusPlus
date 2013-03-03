#ifndef ITEMSCRIPT_H_
#define ITEMSCRIPT_H_

#include <string>

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

    virtual void InitParam(int paramId, int param) {}
    virtual void InitParam(int paramId, float param) {}
    virtual void InitParam(int paramId, const std::string& param) {}
    // Return false if nothing done, return true if item has been used
    virtual bool OnUse(World::EntityPlayer* user, Inventory::ItemStack& item) {return false;}
    virtual bool OnUseOnEntity(World::EntityPlayer* user, World::Entity* target, Inventory::ItemStack& item) {return false;}
    virtual bool OnUseOnBlock(World::EntityPlayer* user, int x, unsigned char y, int z, char face, Inventory::ItemStack& item) {return false;}

    virtual ItemScript* Copy() = 0;
protected:
    Inventory::Item* baseItem;
};

} /* namespace Scripting */
#endif /* ITEMSCRIPT_H_ */
