#ifndef PAINTINGDATA_H_
#define PAINTINGDATA_H_

#include <map>
#include <set>
#include <string>

#include "Util/types.h"
#include "Util/Singleton.h"

namespace Database
{

class PaintingData : public Util::Singleton<PaintingData>
{
    PaintingData();
    friend class Util::Singleton<PaintingData>;
public:
    struct Painting
    {
        bool operator<(const Painting& rhs) const
        {
            return name < rhs.name;
        }
        std::wstring name;
        unsigned short sizeX;
        unsigned short sizeY;
        i_damage itemData;
    };
    const std::set<Painting>& getPainting(i_item itemId);
private:
    void load();
    std::map<i_item, std::set<Painting>> paintingList;
};

} /* namespace Scripting */
#endif /* PAINTINGDATA_H_ */
