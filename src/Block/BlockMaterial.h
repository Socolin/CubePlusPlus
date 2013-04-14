#ifndef MATERIAL_H_
#define MATERIAL_H_

namespace Block
{

class BlockMaterial
{
public:
    BlockMaterial(unsigned char id, bool canBurn, bool replacable, bool translucent, bool requiresNoTool, int mobilityFlag, bool solid, bool liquid);
    BlockMaterial();
    virtual ~BlockMaterial();

    inline unsigned char getId() const
    {
        return id;
    }

    inline bool isCanBurn() const
    {
        return canBurn;
    }

    inline int getMobilityFlag() const
    {
        return mobilityFlag;
    }

    inline bool isReplacable() const
    {
        return replacable;
    }

    inline bool isRequiresNoTool() const
    {
        return requiresNoTool;
    }

    inline bool isTranslucent() const
    {
        return translucent;
    }

    bool isLiquid() const
    {
        return liquid;
    }

    bool isSolid() const
    {
        return solid;
    }

private:
    unsigned char id;
    bool canBurn;
    bool replacable;
    bool translucent;
    bool requiresNoTool;
    int mobilityFlag;
    bool solid;
    bool liquid;
};

} /* namespace Block */
#endif /* MATERIAL_H_ */
