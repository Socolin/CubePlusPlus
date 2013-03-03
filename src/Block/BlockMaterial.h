#ifndef MATERIAL_H_
#define MATERIAL_H_

namespace Block
{

class BlockMaterial
{
public:
    BlockMaterial(bool canBurn, bool replacable, bool translucent, bool requiresNoTool, int mobilityFlag);
    BlockMaterial();
    virtual ~BlockMaterial();

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

private:
    bool canBurn;
    bool replacable;
    bool translucent;
    bool requiresNoTool;
    int mobilityFlag;
};

} /* namespace Scripting */
#endif /* MATERIAL_H_ */
