#include "EntityMetadata.h"

namespace World
{

EntityMetadata::EntityMetadata(int valueId, eDataType type)
    : valueId(valueId)
    , type(type)
{
}

EntityMetadata::~EntityMetadata()
{
}

EntityMetadata::eDataType EntityMetadata::getType() const
{
    return type;
}

} /* namespace World */
