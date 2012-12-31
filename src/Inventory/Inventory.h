#ifndef INVENTORY_H_
#define INVENTORY_H_

namespace Inventory
{
class ItemStack;
class Inventory
{
public:
    Inventory();
    virtual ~Inventory();

    virtual ItemStack& GetSlot(int slotId) = 0;
    virtual void SetSlot(int slotId, const ItemStack& itemStack) = 0;
private:

};

} /* namespace Inventory */
#endif /* INVENTORY_H_ */
