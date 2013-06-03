#ifndef STRINGENTITYMETADATA_H_
#define STRINGENTITYMETADATA_H_

#include "EntityMetadata.h"

#include <string>

namespace World
{

class StringEntityMetadata : public EntityMetadata
{
public:
    StringEntityMetadata(int valueId, const std::wstring& value);
    virtual ~StringEntityMetadata();
    virtual void Write(Network::NetworkPacket& packet) override;
    const std::wstring& getValue() const;
private:
    const std::wstring value;
};

} /* namespace World */
#endif /* STRINGENTITYMETADATA_H_ */
