#ifndef ITEM_TOOL_SCRIPT_H_
#define ITEM_TOOL_SCRIPT_H_

#include "Inventory/Scripts/ItemScript.h"

#include <set>
#include <map>

#define SCRIPTINGPARAM_ITEM_TOOL_TOOL_TYPE 1
#define SCRIPTINGPARAM_ITEM_TOOL_TOOL_MATERIAL 2

namespace Scripting
{
/*
INSERT INTO `script`(`scriptName`,`paramCount`) VALUES ('item_tool',2);
SET @scriptId = LAST_INSERT_ID();
INSERT INTO `script_info`(`scriptId`,`paramId`,`name`,`type`) VALUES (@scriptId,1,'ToolType',1);
INSERT INTO `script_info`(`scriptId`,`paramId`,`name`,`type`) VALUES (@scriptId,2,'ToolMaterial',1);
*/

class ItemToolScript: public ItemScript
{
public:
    ItemToolScript();
    virtual ~ItemToolScript ();
    virtual ItemScript* Copy() override;
    virtual void InitParam(int paramId, int param) override;
    virtual bool CanHarvestBlock(i_block blockId) const override;
    virtual float GetStrengthVsBlock(i_block blockId) const override;
private:
    int toolType;
    int toolMaterial;
    float efficiency;
    int harvestLevel;
    std::set<i_block> efficiencyAgainstBlock;
    std::set<int> efficiencyAgainstMaterial;
    std::map<i_block, int> requiredHarvestLevel;
};
 
} /* namespace Scripting */
#endif /* ITEM_TOOL_SCRIPT_H_ */
