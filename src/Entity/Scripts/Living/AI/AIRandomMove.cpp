#include "AIRandomMove.h"

#include "Entity/Scripts/LivingEntityScript.h"

namespace Scripting
{

AIRandomMove::AIRandomMove()
    : randomMoveTimer(0)
    , notMoving(false)
    , randomMoveSpeed(0.1)
    , baseScript(nullptr)
{
}

AIRandomMove::~AIRandomMove()
{
}

void AIRandomMove::randomMoveInit(LivingEntityScript* script, float speed)
{
    baseScript = script;
    randomMoveSpeed = speed;
    randomMoveUpdateDestination();
}

void AIRandomMove::randomMoveUpdate()
{
    if (randomMoveTimer <= 0)
    {
        randomMoveUpdateDestination();
    }
    else
        randomMoveTimer--;

    if (!notMoving)
        baseScript->MoveToDestination(randomMoveSpeed);
}

void AIRandomMove::randomMoveUpdateDestination()
{
    if (rand() % 3 == 0)
    {
        notMoving = true;
    }
    else
    {
        notMoving = false;
        randomMoveTimer = 100 + (rand() % 200);
        baseScript->GenerateDestination(5);
    }
}


} /* namespace Scripting */

