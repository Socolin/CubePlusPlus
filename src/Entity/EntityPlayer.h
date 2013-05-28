#ifndef ENTITYPLAYER_H_
#define ENTITYPLAYER_H_

#include "LivingEntity.h"
#include "Inventory/InventoryPlayer.h"

#include <boost/heap/binomial_heap.hpp>
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
        GAMEMODE_ADVENTURE
    };

    enum eAction
    {
        ACTION_CROUCH = 1,
        ACTION_UNCROUCH,
        ACTION_LEAVE_BED,
        ACTION_START_SPRINTING,
        ACTION_STOP_SPRINTING,
    };
public:
    EntityPlayer(double x, double y, double z, const std::wstring& name, Network::NetworkSession* session);
    virtual ~EntityPlayer();

    /**
     * Send a packet to player
     * @param packet
     */
    void Send(const Network::NetworkPacket& packet) const;

    /**
     * Add chunk in list to be send to player
     * @param x x chunk coordinate
     * @param z z chunk coordinate
     */
    void AddChunkToSend(int x, int z);

    /**
     * Revive player and teleport him to x, y, z
     * @param x coordinate
     * @param y coordinate
     * @param z coordinate
     */
    void Respawn(double x, double y, double z);

    /**
     * Called when player join a world, it send all initialization packets
     */
    void OnJoinWorld();

    /**
     * Called each world's tick to update entity
     */
    virtual void UpdateTick() override;
    virtual void GetCreatePacket(Network::NetworkPacket& packet) override;
    virtual void GetSpecificUpdatePacket(Network::NetworkPacket& packet) override;
    virtual void moveToVirtualChunk(int newVirtualChunkX, int newVirtualChunkZ) override;
    virtual void moveToChunk(int newChunkX, int newChunkZ) override;

    /**
     * Kick player
     * TODO: add reason and check it work well
     */
    void Kick();

    /**
     * Resend block data to player, ex: when a PlaceBlock fail etc...
     * @param x block coordinate
     * @param y block coordinate
     * @param z block coordinate
     */
    void ResetBlock(int x, unsigned char y, int z);

    /**
     * Called when a player ask to place block
     * @param x from network packet
     * @param y from network packet
     * @param z from network packet
     * @param face from network packet
     * @param cursorPositionX from network packet
     * @param cursorPositionY from network packet
     * @param cursorPositionZ from network packet
     */
    void PlaceBlock(int x, unsigned char y, int z, char face, char cursorPositionX, char cursorPositionY,char cursorPositionZ);

    /**
     * Called when a player ask to dig a block
     * @param state from network packet
     * @param x from network packet
     * @param y from network packet
     * @param z from network packet
     * @param face from network packet
     */
    void DigBlock(int state, int x, unsigned char y, int z, char face);

    void DoAction(char action);

    /**
     * Get player gamemode, survival, creative, adventure
     * @return the player's gamemode
     */
    eGameMode GetGameMode() const;

    /**
     * Get player name
     * @return player's username
     */
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

    /**
     * Notify that player had change item in his hand, so it will send update packet to notify to other player
     */
    void ItemInHandHasChange();

    /**
     * Notify that player has play animation, like using item, it will send packet on next network update
     * @param animationId
     */
    void PlayAnimation(char animationId);

    /**
     * Open window for player, if a player already have an opened window, then it is close
     * @param window the new window object
     */
    void OpenWindow(Window::Window* window);

    /**
     * Close window using window Id
     * @param windowId
     * @param force
     */
    void CloseWindow(i_windowId windowId, bool sendPacket = false);

    /**
     * Called when recieve packet that ask it
     * @param windowId from network packet
     * @param slotId from network packet
     * @param button from network packet
     * @param action from network packet
     * @param mode from network packet
     * @param slot from network packet
     */
    void ClickOnWindow(i_windowId windowId, short slotId, char button, short action, char mode, const Inventory::ItemStack* slot);

    /**
     * Get next windows id and save it
     * @return
     */
    i_windowId GetNextAndSetCurrentWindowId();

    /**
     * Get inventory which contain clicked item, in slot 0
     * @return a non null inventory
     */
    Inventory::Inventory* GetClickedItem() const;

    /**
     * Get the crafting inventory of player, the 2x2 crafting space
     * @return a non null inventory
     */
    Inventory::Inventory* GetCraftingInventory() const;

    /**
     * Get the armor inventory of player, which contain the 4 pieces of armor
     * @return a non null inventory
     */
    Inventory::Inventory* GetArmorInventory() const;

    /**
     * Get the main inventory of player, made with 27 slots
     * @return a non null inventory
     */
    Inventory::Inventory* GetMainInventory() const;

    /**
     * Get the hand inventory of player, made with 9 slots
     * @return a non null inventory
     */
    Inventory::InventoryPlayer* GetHandsInventory() const;
    /**
     * Get the ender chest inventory of player, made with 27 slots
     * @return a non null inventory
     */
    Inventory::Inventory* GetEnderChestInventory() const;

    /**
     * Update slot of main inventory and clicked item
     */
    void UpdateInventories();

    /**
     * Return main window of player, wich has id 0 and made with
     * crafting inventory 2x2
     * main player inventory 36 slots
     * armor inventory
     * This window MUST NOT be closed or open, but can be 're opened'
     * @return window with id = 0
     */
    Window::Window* GetInventoryWindow() const;
private:
    typedef struct
    {
        int x;
        int z;
        int distance;
    } ChunkToSendData;

    struct CompareChunkToSendData
    {
       bool operator() (const ChunkToSendData &a,const ChunkToSendData &b) const
       {
           return (a.distance > b.distance);
       }
     };
private:
    eGameMode gameMode;
    std::wstring name;
    Network::NetworkSession* session;
    boost::heap::binomial_heap<ChunkToSendData,boost::heap::compare<CompareChunkToSendData>> chunkToSend;
    Inventory::Inventory* mainInventory;
    Inventory::InventoryPlayer* handsInventory;
    Inventory::Inventory* enderChestInventory;
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
