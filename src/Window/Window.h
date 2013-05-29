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
    /**
     * Create new window for a player, using windowData
     * @param id unique id of window for player [1-100]
     * @param player player that have open window
     * @param windowData data of window
     */
    Window(i_windowId id, World::EntityPlayer* player, const WindowStaticData* windowData);
    virtual ~Window();

    /**
     * Open window from a bloc at coord x,y,z it try to find tileEntity and call script OnOpenWindow
     * if a tileEntity is found
     * @param x coordinate
     * @param y coordinate
     * @param z coordinate
     */
    void OpenWindow(int x, i_height y, int z);

    /**
     * Open a window and send packet to player, this is send when calling OpenWindow(x,y,z)
     * Send inventories content
     * @param sendOpenPacket
     */
    void OpenWindow(bool sendOpenPacket);

    /**
     * Close window, close inventories
     * @param sendPacket
     */
    void CloseWindow(bool sendPacket);

    /**
     * Called when the player do a click on a window
     * @param slotId id of slot clicked
     * @param button clicked button
     * @param action
     * @param mode
     * @param slot
     * @return true if action is valid
     */
    bool ClickOnWindow(i_slot slotId, char button, short action, char mode, const Inventory::ItemStack* slot);

    /**
     *
     * @param action
     * @param accepted
     */
    void ConfirmTransaction(short action, bool accepted);

    /**
     * Called when a player do an action on window (click on a button)
     * @param action
     */
    void DoAction(short action);

    /**
     * Called to change item in slotId
     * @param slotId id of slot to change
     * @param slot new item
     */
    void SetSlot(i_slot slotId, const Inventory::ItemStack* slot);

    /**
     * Link an inventory to window
     * @param inventory
     * @param priority for shift click order
     */
    void AddInventory(Inventory::Inventory* inventory, ePriority priority = PRIORITY_MEDIUM);

    i_windowId GetId() const;
    const WindowStaticData* GetWindowData() const;

    /**
     * Reopen all inventories and update offset and window id
     * @param player
     */
    void ReOpenAllInventories(World::EntityPlayer* player);

    /**
     * Update all inventories, send slot change data to player
     */
    void UpdateInventories();

    /**
     * Get inventory from slotId of window and return also slot id relative to inventory
     * @param slotId
     * @param inventorySlotId
     * @return
     */
    Inventory::Inventory* GetInventoryForSlot(i_slot slotId, i_slot& inventorySlotId);

    /**
     * Get the inventory which matches with type
     * @param type
     * @return
     */
    Inventory::Inventory* GetInventoryByType(Inventory::eInventoryType type);

    /**
     *
     * @param inventoryFlag
     * @param item
     * @return
     */
    int CountAvaibleSpaceForItem(int inventoryFlag, const Inventory::ItemStack* item);

    /**
     *
     * @return
     */
    const std::vector<Inventory::Inventory*>& GetInventoryListByPriority() const;

    void PlaceAllItemInStackToInventories(int inventoryFlag, Inventory::ItemStack* item, bool reverseOrder);

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
    int maxSlot;

    bool isPainting;
    unsigned char paintingButton;
    short currentPaintingAction;
    std::set<i_slot> paintedSlot;
};

} /* namespace Window */
#endif /* WINDOW_H_ */
