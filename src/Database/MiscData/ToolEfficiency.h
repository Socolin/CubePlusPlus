#ifndef TOOLEFFICIENCY_H_
#define TOOLEFFICIENCY_H_

#include "Util/types.h"
#include "Util/Singleton.h"

#include <set>
#include <map>

namespace Database
{

class ToolEfficiency : public Util::Singleton<ToolEfficiency>
{
    ToolEfficiency();
    friend Util::Singleton<ToolEfficiency>;
public:
    virtual ~ToolEfficiency();

    void InitInstance() override;

    const std::set<i_block>& GetEfficiencyAgainstBlock(int toolTypeId);
    const std::set<int>& GetEfficiencyAgainstMaterial(int toolTypeId);
    const std::map<i_block, int>& GetRequiredHarvestLevel(int toolTypeId);
private:
    void load();
    std::map<int, std::set<i_block> > efficiencyAgainstBlock;
    std::map<int, std::set<int> > efficiencyAgainstMaterial;
    std::map<int, std::map<i_block, int> > requiredHarvestLevel;

};

} /* namespace Database */
#endif /* TOOLEFFICIENCY_H_ */
