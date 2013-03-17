#ifndef ENTITYMETADATA_H_
#define ENTITYMETADATA_H_

namespace Network
{
class NetworkPacket;
}

namespace World
{

class EntityMetadata
{
public:
    enum eDataType
    {
        DATATYPE_CHAR,
        DATATYPE_SHORT,
        DATATYPE_INTEGER,
        DATATYPE_FLOAT,
        DATATYPE_STRING,
        DATATYPE_ITEMSTACK,
        DATATYPE_CHUNKCOORDINATE,
    };
public:
    EntityMetadata(int valueId, eDataType type);
    virtual ~EntityMetadata();
    virtual void Write(Network::NetworkPacket& packet) = 0;
    const eDataType getType() const;
protected:
    int valueId;
    const eDataType type;
};

} /* namespace World */
#endif /* ENTITYMETADATA_H_ */
