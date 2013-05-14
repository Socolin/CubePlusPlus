#ifndef ENTITYPLAYER_H_
#define ENTITYPLAYER_H_

#include "LivingEntity.h"
#include "Inventory/InventoryPlayer.h"

#include <string>
#include <queue>

namespace Network
{
class NetworkSession;
}
namespace Window
{
class Window;
}
namespace World
{
#define DEFAULT_WALKING_SPEED 24
#define DEFAULT_FLYING_SPEED 12

class EntityPlayer: public LivingEntity
{
public:
    enum PlayerAbilities
    {
        DAMAGE_DISABLE = 0X1,
        FLYING = 0X2,
        CAN_FLY = 0X4,
        CREATIVE_MODE = 0X8
    };

    enum eGameMode
    {
        GAMEMODE_SURVIVAL,
        GAMEMODE_CREATVE,
        GAMEMODE_AVENTURE
    };
public:
    EntityPlayer(double x, double y, double z, const std::wstring& name, Network::NetworkSession* session);
    virtual ~EntityPlayer();

    void AddChunkToSend(int x, int z);
    virtual void UpdateTick() override;
    void Respawn(double x, double y, double z);
    void OnJoinWorld();
    void Send(const Network::NetworkPacket& packet) const;
    virtual void GetCreatePacket(Network::NetworkPacket& packet) override;
    virtual void GetSpecificUpdatePacket(Network::NetworkPacket& packet) override;
    virtual void moveToVirtualChunk(int newVirtualChunkX, int newVirtualChunkZ) override;
    virtual void moveToChunk(int newChunkX, int newChunkZ) override;

    void Kick();

    void ResetBlock(int x, unsigned char y, int z);

    void PlaceBlock(int x, unsigned char y, int z, char face, char cursorPositionX, char cursorPositionY,char cursorPositionZ);
    void DigBlock(int state, int x, unsigned char y, int z, char face);

    eGameMode GetGameMode() const;
    const std::wstring& GetUsername();

    /**
     * Drop an item, using player orientation
     * @param itemstack the item stack wich will be linked to entity
     *          this item could be deleted so make a copy before or remove other reference to it
     */
    void DropItem(Inventory::ItemStack* itemstack);

    /**
     * Called when server receive packet that player has click on entity
     * @param target
     * @param leftClick
     */
    void UseEntity(int target, bool leftClick);

    void ItemInHandHasChange();
    void PlayAnimation(char animationId);

    void OpenWindow(Window::Window* window);
    void CloseWindow(i_windowId windowId, bool force = false);
    void ClickOnWindow(i_windowId windowId, short slotId, char button, short action, char mode, const Inventory::ItemStack* slot);
    i_windowId GetCurrentWindow() const;
    void SetCurrentWindow(i_windowId currentWindow);
    i_windowId GetNextAndSetCurrentWindowId();

    /**
     * Get inventory which contain clicked item, in slot 0
     * @return an inventory
     */
    Inventory::Inventory* GetClickedItem() const;
    Inventory::Inventory* GetCraftingInventory() const;
    Inventory::Inventory* GetArmorInventory() const;
    Inventory::InventoryPlayer* GetInventory() const;
    void UpdateInventories();

    Window::Window* GetInventoryWindow() const;
private:
    eGameMode gameMode;
    std::wstring name;
    std::queue<std::pair<int, int> > chunkToSend;
    Network::NetworkSession* session;
    Inventory::InventoryPlayer* inventory;
    Inventory::Inventory* clickedItem;
    Inventory::Inventory* armorInventory;
    Inventory::Inventory* craftingInventory;
    i_windowId currentWindowId;
    char animationId;
    Window::Window* currentWindow;
    Window::Window* inventoryWindow;
};

} /* namespace World */
#endif /* ENTITYPLAYER_H_ */
