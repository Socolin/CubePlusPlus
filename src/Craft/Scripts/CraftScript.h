#ifndef CRAFTSCRIPT_H_
#define CRAFTSCRIPT_H_

#include <string>

#include "Craft/Craft.h"

namespace Craft
{
class Craft;
}
namespace Inventory
{
class InventoryCraft;
}
namespace Scripting
{
class CraftScript
{
public:
    CraftScript(const char* scriptName);
    virtual ~CraftScript();

    virtual void Init(Craft::Craft* baseCraft);

    virtual CraftScript* Copy() = 0;

    virtual void InitParam(int /*paramId*/, int /*param*/) {}
    virtual void InitParam(int /*paramId*/, float /*param*/) {}
    virtual void InitParam(int /*paramId*/, const std::string& /*param*/) {}

    virtual void SetNextSlot(i_item itemId, i_damage itemData);
    virtual bool Match(Inventory::InventoryCraft* craftInventory) const;
protected:
    Craft::Craft* baseCraft;
};

} /* namespace Scripting */
#endif /* CRAFTSCRIPT_H_ */
