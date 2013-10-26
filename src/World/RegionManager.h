#ifndef REGIONMANAGER_H_
#define REGIONMANAGER_H_

#include <unordered_map>
#include <string>

namespace World
{
#define REGION_KEY(X,Z) ((((long long)X << 32) & 0xffffffff00000000)| ((long long)Z & 0x00000000ffffffff))

class Region;
class RegionManager
{
public:
    RegionManager();
    RegionManager(const std::string& worldPath);
    virtual ~RegionManager();
    Region* GetRegion(int x, int z);
private:
    void registerRegion(int x, int z, Region* region);
private:
    std::unordered_map<long long, Region*> regionMap;
    std::string worldPath;
};

} /* namespace World */
#endif /* REGIONMANAGER_H_ */
