#include "Region.h"

#include <iostream>
#include <sstream>
#include <cassert>
#include <cmath>

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

    LOG_DEBUG << "Reading region : " << fileName.str() << std::endl;
    file.open(fileName.str(), std::fstream::in | std::fstream::out | std::fstream::binary);

    if (!file.is_open() || !file.good())
    {
        LOG_ERROR << "Failed to open file :" << fileName.str() << std::endl;
        file.close();
        return;
    }

    // Checking filesize
    file.seekg(0, file.end);
    size_t fileSize = file.tellp();
    file.seekg(0, file.beg);

    LOG_DEBUG << "      Region filesize: " << fileSize << " align:" << (fileSize & 0xfff) << std::endl;

    if (fileSize < REGION_HEADER_SIZE)
    {
        for (size_t pos = 0; pos < REGION_HEADER_SIZE; pos++)
        {
            file << char(0);
        }
        file.seekg(0, file.beg);
        fileSize = REGION_HEADER_SIZE;
    }
    if (fileSize & 0xfff)
    {
        LOG_ERROR << "Region: "  << x << "." << z << ": size was not align on 4096" << std::endl;
        file.seekg(0, file.end);
        size_t countBlankChar = 4096 - (fileSize & 0xfff);
        while (countBlankChar > 1)
        {
            file << char(0);
            countBlankChar--;
        }
        fileSize += countBlankChar;
        file.seekg(0, file.beg);
    }

    // Reading header
    file.seekg(0, file.beg);
    file.read(reinterpret_cast<char*>(locationsTable), REGION_CHUNK_COUNT * sizeof(int));
    file.read(reinterpret_cast<char*>(lastAccessTimeTable), REGION_CHUNK_COUNT * sizeof(int));

    // Compute total available block
    size_t blockCount = fileSize /= REGION_BLOCK_SIZE;
    // Initialize list of free block
    freeBlock.resize(blockCount, true);

    // Two first block are every time busy by header
    freeBlock[0] = false;
    freeBlock[1] = false;

    for (int chunkX = 0; chunkX < REGION_CHUNK_WIDTH; chunkX++)
    {
        for (int chunkZ = 0; chunkZ < REGION_CHUNK_HEIGHT; chunkZ++)
        {
            // Convert from big endian to good endianness
            int value = locationsTable[chunkZ][chunkX].value;
            locationsTable[chunkZ][chunkX].value = be32toh(value);
            int accessTime = lastAccessTimeTable[chunkZ][chunkX];
            lastAccessTimeTable[chunkZ][chunkX] = be32toh(accessTime);

            const offset offset = locationsTable[chunkZ][chunkX];
            if (size_t(offset.data.offsetBlock + offset.data.size) > blockCount)
            {
                LOG_ERROR << "Error while loading region: "  << x << " " << z
                        << " Corrupted file, try to load chunk " << chunkX << " " << chunkZ
                        << " at offset:" << offset.data.offsetBlock << " and with size: " << offset.data.size
                        << " but file only contains " << blockCount << " blocks" << std::endl;
                file.close();
                return;
            }
            if (locationsTable[chunkZ][chunkX].value != 0)
            {
                // Initialize list of busy block
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
    if (opened)
    {
        // Checking filesize
        file.seekg(0, file.end);
        size_t fileSize = file.tellp();
        // Compute total available block
        size_t blockCount = fileSize /= REGION_BLOCK_SIZE;
        if (blockCount != freeBlock.size())
        {
            LOG_ERROR << "When saving region " << regionX << " " << regionZ
                    << " freeBlock != blockCount :" << blockCount << " blocks and "
                    << freeBlock.size() << " free blocks" << std::endl;
        }
        file.close();
    }
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
        LOG_ERROR << "While loading chunk " << int(chunkX) << " " << int(chunkZ)
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
                LOG_ERROR << "While loading chunk " << int(chunkX) << " " << int(chunkZ)
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
    if (!opened)
        return;

    NBT::Buffer chunkData(nbtChunkData);
    if (chunkData.Save())
    {
        uLongf len;
        char* buffer = chunkData.GetCompressedBuffer(len);
        if (buffer == nullptr)
        {
            return;
        }
        int dataSizeBe32 = htobe32(int(len) + 1);
        offset offset = locationsTable[chunkZ][chunkX];
        int neededBlock = std::ceil((static_cast<float>(len + 5) / REGION_BLOCK_SIZE));
        int startBlock = -1;
        char compressionType = 2;
        // Chunk not saved yet
        if (offset.value == 0)
        {
            int countFreeBlock = 0;
            for (size_t i = 2; i < freeBlock.size(); i++)
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
                file.write((char*)&dataSizeBe32, 4);
                file.write(&compressionType, 1);
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
                file.write((char*)&dataSizeBe32, 4);
                file.write(&compressionType, 1);
                file.write(buffer, len);
                int padding = 4096 - ((len + 5) & 0xfff);
                for (int i = 0; i < padding; i++)
                    file << char(0);
            }
        }
        else
        {
            // Chunk already saved, and there is enough space
            if (offset.data.size >= neededBlock)
            {
                for (int blockId = offset.data.offsetBlock; blockId < offset.data.offsetBlock + offset.data.size; blockId++)
                {
                    freeBlock[blockId] = true;
                }
                startBlock = offset.data.offsetBlock;
                offset.data.size = neededBlock;
                file.seekg(startBlock * REGION_BLOCK_SIZE, file.beg);
                file.write((char*)&dataSizeBe32, 4);
                file.write(&compressionType, 1);
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
                file.write((char*)&dataSizeBe32, 4);
                file.write(&compressionType, 1);
                file.write(buffer, len);
                int padding = 4096 - ((len + 5) & 0xfff);
                for (int i = 0; i < padding; i++)
                    file << char(0);
            }
        }
        offset.data.size = neededBlock;
        offset.data.offsetBlock = startBlock;
        locationsTable[chunkZ][chunkX] = offset;
        lastAccessTimeTable[chunkZ][chunkX] = time(nullptr);
        // Writing header
        int toWriteOffset = htobe32(locationsTable[chunkZ][chunkX].value);
        int toWriteTimestamp = htobe32(lastAccessTimeTable[chunkZ][chunkX]);
        file.seekg((chunkZ * 32 + chunkX) * sizeof(int), file.beg);
        file.write(reinterpret_cast<char*>(&toWriteOffset), sizeof(int));
        file.seekg(4096 - sizeof(int), file.cur);
        file.write(reinterpret_cast<char*>(&toWriteTimestamp), sizeof(int));
    }
    else
    {
        LOG_ERROR<< "While saving chunk " << int(chunkX) << " " << int(chunkZ)
            << " in region " << regionX << " " << regionZ << " : "
            << chunkData.GetLastErrorMessage() << std::endl;
    }
}

} /* namespace World */
