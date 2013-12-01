#ifndef TOOLMATERIALS_H_
#define TOOLMATERIALS_H_

#include "Util/Singleton.h"

#include <string>
#include <map>

namespace Database
{
struct ToolMaterial
{
    int id;
    std::string name;
    int harvestLevel;
    int maxUses;
    float efficiency;
    int maxdamageVsEntity;
    int enchantability;
};

class ToolMaterials : public Util::Singleton<ToolMaterials>
{
    ToolMaterials();
    friend Util::Singleton<ToolMaterials>;
public:
    virtual ~ToolMaterials();

    void InitInstance() override;

    const ToolMaterial* GetMaterial(int id);
private:
    void load();
    std::map<int, ToolMaterial*> materialList;
};

} /* namespace Database */
#endif /* TOOLMATERIALS_H_ */
