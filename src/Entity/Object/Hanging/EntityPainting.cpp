#include "EntityPainting.h"

#include "Network/NetworkPacket.h"
#include "Network/OpcodeList.h"

namespace World
{

EntityPainting::EntityPainting(int x, i_height y, int z, int direction, Inventory::ItemStack& item, World* world)
    : EntityHanging(ENTITY_TYPE_PAINTING,  x, y, z, direction)
{
    this->world = world;
    SelectRandomPainting(item);
    this->world = nullptr;
}

EntityPainting::~EntityPainting()
{

}

void EntityPainting::GetCreatePacket(Network::NetworkPacket& packet)
{
    packet << (unsigned char) Network::OP_SPAWN_PAINTING
            << entityId
            << art.name
            << blockX
            << (int)blockY
            << blockZ
            << direction;

    packet << (unsigned char) Network::OP_ENTITY_METADATA
            << entityId;
    metadataManager.Write(packet);
}

void EntityPainting::GetSpecificUpdatePacket(Network::NetworkPacket& packet)
{
}


void EntityPainting::SelectRandomPainting(Inventory::ItemStack& itemStack)
{
    std::set<Database::PaintingData::Painting> paintingList = Database::PaintingData::Instance().getPainting(itemStack.getItemId());
    if (paintingList.empty())
    {
        kill();
        return;
    }
    else
    {
        std::vector<Database::PaintingData::Painting> compatibleArtList;
        for (Database::PaintingData::Painting painting : paintingList)
        {
            if (painting.itemData != itemStack.getItemData())
                continue;
            art = painting;
            setDirection(direction);
            if (isOnValidSurface())
            {
                compatibleArtList.push_back(painting);
            }
        }

        if (compatibleArtList.empty())
        {
            kill();
            return;
        }
        int selectedArt = std::rand() % compatibleArtList.size();
        art = compatibleArtList[selectedArt];
        setDirection(direction);
    }
}

int EntityPainting::getPixelWidth() const
{
    return art.width;
}

int EntityPainting::getPixelHeight() const
{
    return art.height;
}

} /* namespace World */
