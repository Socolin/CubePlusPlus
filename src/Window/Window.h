#ifndef WINDOW_H_
#define WINDOW_H_

#include "Inventory/InventoryConstants.h"
#include "Inventory/ItemStack.h"
#include "Util/types.h"
#include "Window/WindowStaticData.h"

#include <vector>
#include <set>

namespace Scripting
{
class WindowScript;
}

namespace World
{
class EntityPlayer;
}
namespace Inventory
{
class Inventory;
}
namespace Window
{

enum eWindowClickMode
{
    WINDOW_CLICK_MODE_CLICK,
    WINDOW_CLICK_MODE_SHIFT,
    WINDOW_CLICK_MODE_KEYBOARD,
    WINDOW_CLICK_MODE_MIDDLE,
    WINDOW_CLICK_MODE_DROP,
    WINDOW_CLICK_MODE_PAINTING,
    WINDOW_CLICK_MODE_DOUBLECLICK,
};

class Window
{
public:
    enum ePriority
    {
        PRIORITY_MEDIUM,
        PRIORITY_HIGH,
    };
public:
    //TODO: use i_slot
    Window(i_windowId id, World::EntityPlayer* player, const WindowStaticData* windowData);
    virtual ~Window();
    void OpenWindow(int x, i_height y, int z);
    void OpenWindow(bool sendOpenPacket);
    void CloseWindow(bool sendPacket);
    bool ClickOnWindow(short slotId, char button, short action, char mode, const Inventory::ItemStack* slot);
    void ConfirmTransaction(short action, bool accepted);
    void DoAction(short action);

    void SetSlot(short slotId, const Inventory::ItemStack* slot);
    void SetWindowItems(short slotId, const Inventory::ItemStack* slot);

    void AddInventory(Inventory::Inventory* inventory, ePriority priority = PRIORITY_MEDIUM);

    i_windowId GetId() const;
    const WindowStaticData* GetWindowData() const;

    void ReOpenAllInventories(World::EntityPlayer* player);
    void UpdateInventories();

    Inventory::Inventory* GetInventoryForSlot(i_slot slotId, i_slot& inventorySlotId);
    Inventory::Inventory* GetInventoryByType(Inventory::eInventoryType type);

    int CountAvaibleSpaceForItem(int inventoryFlag, const Inventory::ItemStack* item);
    const std::vector<Inventory::Inventory*>& GetInventoryListByPriority() const;

private:
    enum ePaintingAction
    {
        PAINTING_ACTION_START,
        PAINTING_ACTION_PROGRESS,
        PAINTING_ACTION_END,
    };
private:
    bool startPainting(int mouseButton, int action);
    bool progressPainting(i_slot slotId, int action);
    bool endPainting(int action);

private:
    const i_windowId id;
    World::EntityPlayer* player;
    Scripting::WindowScript* script;
    std::vector<Inventory::Inventory*> inventoryList;
    std::vector<Inventory::Inventory*> inventoryListByPriority;
    const WindowStaticData* windowData;
    int offset;

    bool isPainting;
    unsigned char paintingButton;
    short currentPaintingAction;
    std::set<i_slot> paintedSlot;
};

} /* namespace Window */
#endif /* WINDOW_H_ */
