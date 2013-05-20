#ifndef CRAFTMANAGER_H_
#define CRAFTMANAGER_H_

#include "Util/Singleton.h"

#include <map>
#include <vector>

#include "CraftContants.h"

namespace Craft
{
class Craft;
#define CRAFT_KEY(width, height) ((width << 4) | height)

class CraftManager : public Util::Singleton<CraftManager>
{
    friend Util::Singleton<CraftManager>;
    CraftManager();
public:
    virtual ~CraftManager();
    void InitInstance() override;
    const std::vector<Craft*>& GetCraftList(int width, int height);
private:
    void load();
    void loadCraftSlot(Craft* craft, int craftId);
    std::map<int, std::vector<Craft*>> craftList;
};

} /* namespace Craft */
#endif /* CRAFTMANAGER_H_ */
