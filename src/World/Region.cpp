#include "Region.h"

#include <iostream>
#include <sstream>
#include <cassert>

#include <cppnbt.h>

#include "Util/StringUtil.h"

namespace World
{

Region::Region(const std::string& worldPath, int x, int z)
    : regionX(x)
    , regionZ(z)
    , opened(false)
{
    std::stringstream fileName;
    fileName << worldPath << "region/" << "r." << x << "." << z << ".mca";

    file.open(fileName.str(), std::fstream::in | std::fstream::out | std::fstream::binary);

    if (!file.is_open())
    {
        std::cerr << "Failed to open file :" << fileName.str() << std::endl;
        return;
    }

    // Checking filesize
    file.seekg(0, file.end);
    size_t fileSize = file.tellp();
    file.seekg(0, file.beg);

    if (fileSize < REGION_HEADER_SIZE)
    {
        for (size_t pos = 0; pos < REGION_HEADER_SIZE; pos++)
        {
            file << int(0);
        }
        file.seekg(0, file.beg);
        fileSize = REGION_HEADER_SIZE;
    }

    if (fileSize & 0xfff)
    {
        size_t countBlankChar = 4096 - (fileSize & 0xfff);
        while (countBlankChar > 4)
        {
            file << int(0);
            countBlankChar += 4;
        }
        while (countBlankChar > 1)
        {
            file << char(0);
            countBlankChar++;
        }
        fileSize += countBlankChar;
        file.seekg(0, file.beg);
    }

    // Reading header
    file.read(reinterpret_cast<char*>(locationsTable), REGION_CHUNK_COUNT * sizeof(int));
    file.read(reinterpret_cast<char*>(lastAccessTimeTable), REGION_CHUNK_COUNT * sizeof(int));

    // Compute total available block
    size_t blockCount = fileSize /= REGION_BLOCK_SIZE;
    // Initialize list of free block
    freeBlock.resize(blockCount, true);

    // Two first block are every time busy by header
    freeBlock[0] = false;
    freeBlock[1] = false;

    for (int x = 0; x < REGION_CHUNK_WIDTH; x++)
    {
        for (int z = 0; z < REGION_CHUNK_HEIGHT; z++)
        {
            // Convert from big endian to good endianness
            int value = locationsTable[z][x].value;
            locationsTable[z][x].value = be32toh(value);
            int accessTime = lastAccessTimeTable[z][x];
            lastAccessTimeTable[z][x] = be32toh(accessTime);

            if (locationsTable[z][x].value != 0)
            {
                // Initialize list of busy block
                const offset& offset = locationsTable[z][x];
                for (int blockId = offset.data.offsetBlock; blockId < offset.data.offsetBlock + offset.data.size; blockId++)
                {
                    freeBlock[blockId] = false;
                }
            }
        }
    }
    opened = true;
}

Region::~Region()
{
    file.close();
}

nbt::NbtBuffer* Region::GetNbtChunkData(i_small_coord chunkX, i_small_coord chunkZ)
{
    if (!opened)
        return nullptr;
    assert(chunkX < 32);
    assert(chunkZ < 32);

    const offset& offset = locationsTable[chunkZ][chunkX];
    if (offset.value == 0)
    {
        return nullptr;
    }

    file.seekg(offset.data.offsetBlock * REGION_BLOCK_SIZE, file.beg);

    int dataSize = 0;
    char type = 0;
    file.read(reinterpret_cast<char*>(&dataSize), sizeof(int));
    file.read(&type, sizeof(char));
    dataSize = be32toh(dataSize);

    if (dataSize <= 0)
        return nullptr;

    if (type != 2)
        return nullptr;

    uint8_t* buffer = new uint8_t[dataSize - 1];
    file.read(reinterpret_cast<char*>(buffer), dataSize - 1);

    nbt::NbtBuffer* chunkData = new nbt::NbtBuffer(buffer, dataSize - 1);
    delete[] buffer;
    return chunkData;
}

} /* namespace World */
