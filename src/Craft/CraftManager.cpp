#include "CraftManager.h"

namespace Craft
{

CraftManager::CraftManager()
{
}

CraftManager::~CraftManager()
{
}

void CraftManager::InitInstance()
{
    load();
}

const std::vector<Craft*>& CraftManager::GetCraftList(int width, int height)
{
    return craftList[CRAFT_KEY(width,height)];
}

void CraftManager::load()
{
}

} /* namespace Craft */
