#include "RegionManager.h"

#include "Region.h"

namespace World
{

RegionManager::RegionManager(const std::string& worldPath)
    : worldPath(worldPath)
{
}

RegionManager::~RegionManager()
{
    for (auto itr : regionMap)
    {
        delete itr.second;
    }
}

void RegionManager::registerRegion(int x, int z, Region* region)
{
    regionMap[REGION_KEY(x, z)] = region;
}

Region* RegionManager::GetRegion(int x, int z)
{
    auto regionItr = regionMap.find(REGION_KEY(x,z));
    if (regionItr != regionMap.end())
    {
        return regionItr->second;
    }

    Region* region = new Region(worldPath, x, z);

    registerRegion(x, z, region);

    return region;
}

} /* namespace World */
