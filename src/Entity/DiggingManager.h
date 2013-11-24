#ifndef DIGGINGMANAGER_H_
#define DIGGINGMANAGER_H_

#include "Util/types.h"

namespace Block
{
class Block;
}
namespace World
{
class EntityPlayer;
class World;
class DiggingManager
{
public:
    DiggingManager(EntityPlayer* player);
    virtual ~DiggingManager();

    void Update();

    void StartDigging(int x, i_height y, int z);
    void EndDigging();
    void StopDigging();

private:
    float getDamageDonePerTickAgainstBlock(const Block::Block* block);
private:
    EntityPlayer* player;
    bool diggingBlock;
    bool playerFinishDigging;
    float diggingStep;
    float diggingProgress;
    int x;
    i_height y;
    int z;
};

} /* namespace World */
#endif /* DIGGINGMANAGER_H_ */
