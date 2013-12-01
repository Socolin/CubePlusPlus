#include "ItemToolScript.h"

#include "Logging/Logger.h"
#include "Database/MiscData/ToolEfficiency.h"
#include "Database/MiscData/ToolMaterials.h"
#include "Block/Block.h"
#include "Block/BlockList.h"

namespace Scripting
{

ItemToolScript::ItemToolScript()
    : ItemScript("item_tool")
    , toolType(0)
    , toolMaterial(0)
    , efficiency(1.f)
    , harvestLevel(0)
{
}

ItemToolScript::~ItemToolScript()
{
}

ItemScript* ItemToolScript::Copy()
{
    return new ItemToolScript(*this);
}

void ItemToolScript::InitParam(int paramId, int param)
{
    switch (paramId)
    {
    case SCRIPTINGPARAM_ITEM_TOOL_TOOL_TYPE:
    {
        toolType = param;
        Database::ToolEfficiency& toolEfficiencyManager = Database::ToolEfficiency::Instance();
        for (int i : toolEfficiencyManager.GetEfficiencyAgainstBlock(toolType))
        {
            LOG_DEBUG << i << std::endl;
        }
        efficiencyAgainstBlock = toolEfficiencyManager.GetEfficiencyAgainstBlock(toolType);
        efficiencyAgainstMaterial = toolEfficiencyManager.GetEfficiencyAgainstMaterial(toolType);
        requiredHarvestLevel = toolEfficiencyManager.GetRequiredHarvestLevel(toolType);
        break;
    }
    case SCRIPTINGPARAM_ITEM_TOOL_TOOL_MATERIAL:
    {
        toolMaterial = param;
        const Database::ToolMaterial* material = Database::ToolMaterials::Instance().GetMaterial(toolMaterial);
        if (material)
        {
            efficiency = material->efficiency;
            harvestLevel = material->harvestLevel;
        }
        break;
    }
    default:
        LOG_ERROR << "Script: ItemToolScript: Bad param id : " << paramId << std::endl;
        break;
    }
}

bool ItemToolScript::CanHarvestBlock(i_block blockId) const
{
    auto itr = requiredHarvestLevel.find(blockId);
    if (itr != requiredHarvestLevel.end())
    {
        int requireLevel = itr->second;
        return harvestLevel >= requireLevel;
    }
    return false;
}

float ItemToolScript::GetStrengthVsBlock(i_block blockId) const
{
    if (efficiencyAgainstBlock.find(blockId) != efficiencyAgainstBlock.end())
        return efficiency;
    const Block::Block* block = Block::BlockList::getBlock(blockId);
    if (block)
    {
        if (efficiencyAgainstMaterial.find(block->GetMaterial().getId()) != efficiencyAgainstMaterial.end())
            return efficiency;
    }
    return 1.0f;
}


} /* namespace Scripting */
