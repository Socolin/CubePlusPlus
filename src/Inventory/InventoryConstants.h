#ifndef INVENTORYCONSTANTS_H_
#define INVENTORYCONSTANTS_H_

namespace Inventory
{
enum eInventoryType
{
    INVENTORY_TYPE_DEFAULT          = 0x1,
    INVENTORY_TYPE_PLAYER_HANDS     = 0x2,
    INVENTORY_TYPE_PLAYER_MAIN      = 0x4,
    INVENTORY_TYPE_FURNACE          = 0x8,
    INVENTORY_TYPE_CRAFT            = 0x10,
};

}

#endif /* INVENTORYCONSTANTS_H_ */
