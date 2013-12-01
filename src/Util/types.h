#ifndef TYPES_H_
#define TYPES_H_

typedef unsigned short i_block;
typedef unsigned short i_item;
typedef unsigned short i_damage;
typedef unsigned char i_lightvalue;
typedef unsigned char i_lightopacity;
typedef unsigned char i_data;
typedef unsigned char i_height;
typedef unsigned char i_small_coord; // Use to represent x and z < 16
typedef unsigned char i_powerlevel;
typedef short i_slot;
typedef int i_stackSize;
typedef unsigned char i_windowId;
typedef int i_windowDataId;
typedef int i_entityId;
typedef struct
{
    i_data blockData;
    i_block blockId;
} s_block_data;

typedef struct block_position
{
    block_position(int x, i_height y, int z)
        : x(x), y(y), z(z)
    {}
    int x;
    i_height y;
    int z;
} s_block_position;

// Return value when we use item.
struct ItemUseResult
{
    bool used; // Used or not
    bool damage; // Damage item is this is at 1, else remove amount of item from stack.
    unsigned short amount; // Quantity of item used, or how many damage must be done on item.
};

#endif /* TYPES_H_ */
