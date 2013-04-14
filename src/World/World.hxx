#ifndef WORLD_HXX_
#define WORLD_HXX_

#include "Block/Block.h"
#include "Block/BlockConstants.h"
#include "Block/BlockList.h"

namespace World
{

inline Chunk* World::GetChunk(int x, int z)
{
    auto it = chunkMap.find(CHUNK_KEY(x,z));
    if (it == chunkMap.end())
    {
        Chunk* chunk = LoadChunk(x, z);
        chunkMap[CHUNK_KEY(x,z)] = chunk;
        return chunk;
    }
    return it->second;
}

inline Chunk* World::GetChunkIfLoaded(int x, int z) const
{
    auto it = chunkMap.find(CHUNK_KEY(x,z));
    if (it == chunkMap.end())
    {
        return NULL;
    }
    return it->second;
}

inline VirtualChunk* World::GetVirtualChunk(int x, int z)
{
    auto it = virtualChunkMap.find(CHUNK_KEY(x,z));
    if (it == virtualChunkMap.end())
    {
        VirtualChunk* vChunk = CreateVirtualChunk(x, z);
        virtualChunkMap[CHUNK_KEY(x,z)] = vChunk;
        return vChunk;
    }
    return it->second;
}

inline VirtualChunk* World::GetVirtualChunkIfLoaded(int x, int z) const
{
    auto it = virtualChunkMap.find(CHUNK_KEY(x,z));
    if (it == virtualChunkMap.end())
    {
        return nullptr;
    }
    return it->second;
}

inline VirtualSmallChunk* World::GetVirtualSmallChunk(int x, int z)
{
    auto it = virtualSmallChunkMap.find(CHUNK_KEY(x,z));
    if (it == virtualSmallChunkMap.end())
    {
        VirtualSmallChunk* vChunk = CreateVirtualSmallChunk(x, z);
        virtualSmallChunkMap[CHUNK_KEY(x,z)] = vChunk;
        return vChunk;
    }
    return it->second;
}

inline VirtualSmallChunk* World::GetVirtualSmallChunkIfLoaded(int x, int z) const
{
    auto it = virtualSmallChunkMap.find(CHUNK_KEY(x,z));
    if (it == virtualSmallChunkMap.end())
    {
        return nullptr;
    }
    return it->second;
}

inline i_block World::GetBlockId(int x, i_height y, int z)
{
    Chunk* chunk = GetChunkIfLoaded(x >> 4, z >> 4);
    if (chunk == nullptr)
        return 0;
    return chunk->getBlockAt(x & 0xf, y, z & 0xf);
}

inline i_data World::GetBlockData(int x, i_height y, int z)
{
    Chunk* chunk = GetChunkIfLoaded(x >> 4, z >> 4);
    if (chunk == nullptr)
        return 0;
    return chunk->getDataAt(x & 0xf, y, z & 0xf);
}

inline s_block_data World::GetBlockIdAndData(int x, i_height y, int z) const
{
    Chunk* chunk = GetChunkIfLoaded(x >> 4, z >> 4);
    s_block_data block_data;
    if (chunk != nullptr)
    {
        block_data.blockId = chunk->getBlockAt(x & 0xf, y, z & 0xf);
        block_data.blockData = chunk->getDataAt(x & 0xf, y, z & 0xf);
    }
    else
    {
        block_data.blockId = 0;
    }
    return block_data;
}
bool World::IsFullBlock(int x, i_height y, int z)
{
    i_block blockId = GetBlockId(x, y, z);
    if (blockId < BLOCK_COUNT)
    {
        const Block::Block* block = Block::BlockList::getBlock(blockId);
        if (block)
        {
            return block->IsFullBlock();
        }
    }
    return false;
}

}

#endif
