#ifndef ITEMLIST_H_
#define ITEMLIST_H_

#define ITEM_COUNT 32000

namespace Inventory
{
class Item;

class ItemList
{
public:
    static void InitInstance()
    {
        instance = new ItemList();
        instance->Load();
    }
    static ItemList* Instance()
    {
        return instance;
    }

    void Load();
private:
    ItemList();
    static ItemList* instance;
    virtual ~ItemList();
    void GenerateItemBlock();
    void LoadScripts();
public:
    Item* items[ITEM_COUNT];

};

} /* namespace Util */
#endif /* ITEMLIST_H_ */
