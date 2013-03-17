#ifndef ITEMSTACKENTITYMETADATA_H_
#define ITEMSTACKENTITYMETADATA_H_

#include "EntityMetadata.h"
#include "Inventory/ItemStack.h"

namespace World
{
class ItemStackEntityMetadata : public EntityMetadata
{
public:
    ItemStackEntityMetadata(int valueId,const Inventory::ItemStack& value);
    virtual ~ItemStackEntityMetadata();
    virtual void Write(Network::NetworkPacket& packet) override;
    const Inventory::ItemStack& getValue() const;

private:
    const Inventory::ItemStack value;
};

} /* namespace World */
#endif /* ITEMSTACKENTITYMETADATA_H_ */
