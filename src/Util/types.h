#ifndef TYPES_H_
#define TYPES_H_

typedef unsigned short i_block;
typedef unsigned short i_item;
typedef unsigned short i_damage;
typedef unsigned char i_data;
typedef unsigned char i_height;
typedef struct
{
    i_data blockData;
    i_block blockId;
} s_block_data;

#endif /* TYPES_H_ */
