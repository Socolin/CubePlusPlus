#ifndef ITEMLIST_H_
#define ITEMLIST_H_

#include "Util/Singleton.h"

#include "Util/types.h"

#define ITEM_COUNT 32000

namespace Inventory
{
class Item;

class ItemList : public Util::Singleton<ItemList>
{
    friend class Util::Singleton<ItemList>;
public:
    static const Item* getItem(i_item item)
    {
        return Instance().items[item];
    }
    virtual ~ItemList();
    void InitInstance() override;
private:
    ItemList();
    void Initialize();
    void GenerateItemBlock();
    void LoadScripts();
public:
    Item* items[ITEM_COUNT];

};

} /* namespace Inventory */
#endif /* ITEMLIST_H_ */
