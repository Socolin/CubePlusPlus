#include "Region.h"

#include <iostream>
#include <sstream>
#include <cassert>

#include <NBTField/NBTField.h>

#include "Logging/Logger.h"
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
        LOG_ERROR << "Failed to open file :" << fileName.str() << std::endl;
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

NBT::TagCompound* Region::GetNbtChunkData(i_small_coord chunkX, i_small_coord chunkZ)
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

    NBT::TagCompound* rootAsCompound = nullptr;
    NBT::Buffer chunkData(buffer, dataSize - 1);
    if (!chunkData.Load())
    {
        LOG_ERROR << "While loading chunk " << chunkX << " " << chunkZ
                << " in region " << regionX << " " << regionZ << " : "
                << chunkData.GetLastErrorMessage() << std::endl;
    }
    else
    {
        NBT::Tag* root = chunkData.TakeRoot();
        if (root != nullptr)
        {
            rootAsCompound = root->GetTagAs<NBT::TagCompound>();
            if (rootAsCompound == nullptr)
            {
                LOG_ERROR << "While loading chunk " << chunkX << " " << chunkZ
                    << " in region " << regionX << " " << regionZ << " : "
                    << "Root tag was not a TagCompound but : " << root->GetType() << std::endl;
                delete root;
            }
        }
    }
    delete[] buffer;

    return rootAsCompound;
}

void Region::SaveNbtChunkData(i_small_coord chunkX, i_small_coord chunkZ, NBT::TagCompound* nbtChunkData)
{
    NBT::Buffer chunkData(nbtChunkData);
    if (chunkData.Save())
    {
        uLongf len;
        char* buffer = chunkData.GetCompressedBuffer(len);
        offset offset = locationsTable[chunkZ][chunkX];
        int neededBlock = ((len / REGION_BLOCK_SIZE) + 1);
        int startBlock = -1;
        // Chunk not saved yet
        if (offset.value == 0)
        {
            int countFreeBlock = 0;
            for (int i = 2; i < freeBlock.size(); i++)
            {
                if (freeBlock[i])
                {
                    countFreeBlock++;
                    if (startBlock == -1)
                    {
                        startBlock = i;
                    }
                }
                else
                {
                    startBlock = -1;
                    countFreeBlock = 0;
                }

                if (countFreeBlock >= neededBlock)
                {
                    break;
                }
            }
            // We found space to write
            if (startBlock >= 0 && countFreeBlock >= neededBlock)
            {
                for (int i = startBlock; i < startBlock + neededBlock; i++)
                {
                    freeBlock[i] = false;
                }
                file.seekg(startBlock * REGION_BLOCK_SIZE, file.beg);
                file.write(buffer, len);
            }
            else // No space left, we have to write at end of file
            {
                startBlock = freeBlock.size();
                for (int i = 0; i < neededBlock; i++)
                {
                    freeBlock.push_back(false);
                }
                file.seekg(0, file.end);
                file.write(buffer, len);
            }
        }
        else
        {
            // Chunk already saved, and there is enough space
            if (offset.data.size * REGION_BLOCK_SIZE >= len)
            {
                for (int blockId = offset.data.offsetBlock; blockId < offset.data.offsetBlock + offset.data.size; blockId++)
                {
                    freeBlock[blockId] = true;
                }
                startBlock = offset.data.offsetBlock;
                offset.data.size = neededBlock;
                file.seekg(startBlock * REGION_BLOCK_SIZE, file.beg);
                file.write(buffer, len);
                for (int blockId = offset.data.offsetBlock; blockId < offset.data.offsetBlock + offset.data.size; blockId++)
                {
                    freeBlock[blockId] = false;
                }
            }
            else // Not enough space
            {
                // Free old used space
                for (int blockId = offset.data.offsetBlock; blockId < offset.data.offsetBlock + offset.data.size; blockId++)
                {
                    freeBlock[blockId] = true;
                }
                startBlock = freeBlock.size();
                for (int i = 0; i < neededBlock; i++)
                {
                    freeBlock.push_back(false);
                }
                file.seekg(0, file.end);
                file.write(buffer, len);
            }
        }
        offset.data.size = neededBlock;
        offset.data.offsetBlock = startBlock;
        locationsTable[chunkZ][chunkX] = offset;
        lastAccessTimeTable[chunkZ][chunkX] = time(0);
        file.seekg(0, file.beg);
        // Writing header
        file.write(reinterpret_cast<char*>(locationsTable), REGION_CHUNK_COUNT * sizeof(int));
        file.write(reinterpret_cast<char*>(lastAccessTimeTable), REGION_CHUNK_COUNT * sizeof(int));
    }
    else
    {
        LOG_ERROR<< "While saving chunk " << chunkX << " " << chunkZ
            << " in region " << regionX << " " << regionZ << " : "
            << chunkData.GetLastErrorMessage() << std::endl;
    }
}

} /* namespace World */
