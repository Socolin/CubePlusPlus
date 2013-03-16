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
    EntityMetadata(int valueId);
    virtual ~EntityMetadata();
    virtual void Write(Network::NetworkPacket& packet) = 0;
protected:
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
    int valueId;
};

} /* namespace World */
#endif /* ENTITYMETADATA_H_ */
