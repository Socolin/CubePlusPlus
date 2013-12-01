#include "ToolMaterials.h"

#include "Database/DatabaseManager.h"
#include "Logging/Logger.h"

namespace Database
{

ToolMaterials::ToolMaterials()
{
}

ToolMaterials::~ToolMaterials()
{
}

const ToolMaterial* ToolMaterials::GetMaterial(int id)
{
    return materialList[id];
}

void ToolMaterials::InitInstance()
{
    load();
}

void ToolMaterials::load()
{
    Database::DatabaseManager* db = Database::DatabaseManager::Instance();
    db->connect();
    sql::ResultSet* result = db->querry("SELECT * FROM `tool_material`");

    if (result == nullptr)
    {
        LOG_ERROR << "ERROR: no tool material found in database" << std::endl;
        return;
    }

    LOG_INFO << "Loading tool material datas" << std::endl;
    LOG_DEBUG << Logging::BOLD_BLUE
              << "id" << "\t\t"
              << "name" << "\t"
              << "harves" << "\t"
              << "effici" << "\t"
              << "damag" << "\t"
              << "enchant" << "\t"
              << std::endl;

    while(result->next())
    {
        int id = result->getInt(TableToolMaterial::id);
        std::string name = result->getString(TableToolMaterial::name);
        int harvestLevel = result->getInt(TableToolMaterial::harvestLevel);
        int maxUses = result->getInt(TableToolMaterial::maxUses);
        float efficiency = result->getDouble(TableToolMaterial::efficiency);
        int damageVsEntity = result->getInt(TableToolMaterial::damageVsEntity);
        int enchantability = result->getInt(TableToolMaterial::enchantability);

        LOG_DEBUG << id << "\t"
                << name << "\t"
                << harvestLevel << "\t"
                << maxUses << "\t"
                << efficiency << "\t"
                << damageVsEntity << "\t"
                << enchantability << std::endl;

        ToolMaterial* material = new ToolMaterial{id, name, harvestLevel, maxUses, efficiency, damageVsEntity, enchantability};
        materialList[id] = material;
    }

    delete result;
}

} /* namespace Database */
