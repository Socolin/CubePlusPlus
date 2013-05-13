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
#define DEFAULT_WALKING_SPEED 12
#define DEFAULT_FLYING_SPEED 24
class EntityPlayer: public LivingEntity
{
    enum PlayerAbilities
    {
        DAMAGE_DISABLE = 0X1,
        FLYING = 0X2,
        CAN_FLY = 0X4,
        CREATIVE_MODE = 0X8
    };
public:
    EntityPlayer(double x, double y, double z, const std::wstring& name, Network::NetworkSession* session);
    virtual ~EntityPlayer();

    void AddChunkToSend(int x, int z);
    virtual void UpdateTick();
    void Respawn(double x, double y, double z);
    void OnJoinWorld();
    void Send(const Network::NetworkPacket& packet) const;
    virtual void GetCreatePacket(Network::NetworkPacket& packet);
    virtual void moveToVirtualChunk(int newVirtualChunkX, int newVirtualChunkZ);
    virtual void moveToChunk(int newChunkX, int newChunkZ);

    void Kick();

    void ResetBlock(int x, unsigned char y, int z);

    const std::wstring& GetUsername();
    Inventory::InventoryPlayer& GetInventory();
    void PlaceBlock(int x, unsigned char y, int z, char face, char cursorPositionX, char cursorPositionY,char cursorPositionZ);
    void DigBlock(int state, int x, unsigned char y, int z, char face);
    /**
     * Drop an item, using player orientation
     * @param itemstack the item stack wich will be linked to entity
     *          this item could be deleted so make a copy before or remove other reference to it
     */
    void DropItem(Inventory::ItemStack* itemstack);
    virtual void GetSpecificUpdatePacket(Network::NetworkPacket& packet);

    void UseEntity(int target, bool leftClick);

    void ItemInHandHasChange()
    {
        hasChangeItemInHand = true;
    }
    void PlayAnimation(char animationId);
    Inventory::Inventory& GetClickedItem();
    i_windowId GetCurrentWindow() const;
    i_windowId GetNextAndSetCurrentWindowId();
    void SetCurrentWindow(i_windowId currentWindow);

    void OpenWindow(Window::Window* window);
    void CloseWindow(i_windowId windowId);
    void ClickOnWindow(i_windowId windowId, short slotId, char button, short action, char mode, const Inventory::ItemStack* slot);

private:
    std::wstring name;
    std::queue<std::pair<int, int> > chunkToSend;
    Network::NetworkSession* session;
    Inventory::InventoryPlayer inventory;
    Inventory::Inventory clickedItem;
    i_windowId currentWindowId;
    char animationId;
    Window::Window* currentWindow;
};

} /* namespace World */
#endif /* ENTITYPLAYER_H_ */
