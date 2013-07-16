#include "PaintingData.h"

#include "Database/DatabaseManager.h"
#include "Logging/Logger.h"
#include "Util/StringUtil.h"

namespace Database
{

PaintingData::PaintingData()
{
}

const std::set<PaintingData::Painting>& PaintingData::getPainting(i_item itemId)
{
    return paintingList[itemId];
}

void PaintingData::InitInstance()
{
    load();
}

void PaintingData::load()
{
    Database::DatabaseManager* db = Database::DatabaseManager::Instance();
    db->connect();
    sql::ResultSet* result = db->querry("SELECT * FROM `painting` ORDER BY `name`");

    if (result == nullptr)
    {
        LOG_ERROR << "ERROR: no paintings found in database" << std::endl;
        return;
    }

    LOG_INFO << "Loading paitings's datas" << std::endl;
    LOG_DEBUG << UTIL_TEXT_SHELL_BOLD_BLUE
              << "title" << "\t\t"
              << "sizeX" << "\t"
              << "sizeY" << "\t"
              << "itemId" << "\t"
              << "itemData" << "\t"
              << UTIL_TEXT_SHELL_NONE
              << std::endl;

    while(result->next())
    {
        std::string name = result->getString(TablePainting::name);
        unsigned short sizeX = result->getInt(TablePainting::sizeX);
        unsigned short sizeY = result->getInt(TablePainting::sizeY);
        i_item itemId = result->getInt(TablePainting::itemId);
        i_damage itemData = result->getInt(TablePainting::itemData);

        LOG_DEBUG
                << name << (name.length() < 8 ? "\t" : "") <<  "\t"
                << sizeX << "\t"
                << sizeY << "\t"
                << itemId << "\t"
                << itemData << "\t"
                << std::endl;

        std::wstring wname;
        Util::StringToWString(wname, name);
        paintingList[itemId].insert({wname, sizeX, sizeY, itemData});
    }

    delete result;
}

} /* namespace Database */
