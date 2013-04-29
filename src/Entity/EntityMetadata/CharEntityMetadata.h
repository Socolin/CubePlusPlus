#ifndef CHARENTITYMETADATA_H_
#define CHARENTITYMETADATA_H_

#include "EntityMetadata.h"

namespace World
{
class CharEntityMetadata : public EntityMetadata
{
public:
    CharEntityMetadata(int valueId, char value);
    virtual ~CharEntityMetadata();
    virtual void Write(Network::NetworkPacket& packet) override;
    char getValue() const;
private:
    const char value;
};

} /* namespace World */
#endif /* CHARENTITYMETADATA_H_ */
