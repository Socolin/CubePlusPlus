#include "ToolEfficiency.h"

#include "Database/DatabaseManager.h"
#include "Logging/Logger.h"

namespace Database
{

ToolEfficiency::ToolEfficiency()
{
}

ToolEfficiency::~ToolEfficiency()
{
}

const std::set<i_block>& ToolEfficiency::GetEfficiencyAgainstBlock(int toolTypeId)
{
    return efficiencyAgainstBlock[toolTypeId];
}

const std::set<int>& ToolEfficiency::GetEfficiencyAgainstMaterial(int toolTypeId)
{
    return efficiencyAgainstMaterial[toolTypeId];
}

const std::map<i_block, int>& ToolEfficiency::GetRequiredHarvestLevel(int toolTypeId)
{
    return requiredHarvestLevel[toolTypeId];
}

void ToolEfficiency::InitInstance()
{
    load();
}

void ToolEfficiency::load()
{
    Database::DatabaseManager& db = Database::DatabaseManager::Instance();
    db.connect();
    sql::ResultSet* result = db.querry("SELECT * FROM `tool_efficiency`");

    if (result == nullptr)
    {
        LOG_ERROR << "ERROR: no tool material found in database" << std::endl;
        return;
    }

    LOG_INFO << "Loading tool efficiency datas" << std::endl;

    while(result->next())
    {
        int toolTypeId = result->getInt(TableToolEfficiency::toolTypeId);
        int blockId = result->getInt(TableToolEfficiency::blockId);
        int materialId = result->getInt(TableToolEfficiency::materialId);
        int requireHarvestLevel = result->getInt(TableToolEfficiency::requireHarvestLevel);

        if (blockId)
        {
            auto& effAgainstBlockSet = efficiencyAgainstBlock[toolTypeId];
            effAgainstBlockSet.insert(blockId);

            auto& mapRequireHarvestLevel = requiredHarvestLevel[toolTypeId];
            mapRequireHarvestLevel[blockId] = requireHarvestLevel;
        }
        else if (materialId)
        {
            auto& effAgainstMaterialSet = efficiencyAgainstMaterial[toolTypeId];
            effAgainstMaterialSet.insert(materialId);
        }
    }

    delete result;
}

} /* namespace Database */
