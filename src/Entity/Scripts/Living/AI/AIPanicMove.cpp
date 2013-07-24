#include "AIPanicMove.h"

#include "Entity/Scripts/LivingEntityScript.h"

namespace Scripting
{

AIPanicMove::AIPanicMove()
    : panic(false)
    , panicTimer(0)
    , panicSpeed(0.2)
    , panicNextRandomTick(0)
    , baseScript(nullptr)
{
}

AIPanicMove::~AIPanicMove()
{
}

void AIPanicMove::panicMoveInit(LivingEntityScript* script)
{
    baseScript = script;
}

void AIPanicMove::panicMoveStart()
{
    panic= true;
    panicTimer = 60;
    baseScript->GenerateDestination(10);
}

void AIPanicMove::panicMoveUpdate()
{
    if (panicTimer <= 0)
    {
        panic = false;
    }
    else
        panicTimer--;


    if (panicNextRandomTick <= 0)
    {
        baseScript->GenerateDestination(10);
        panicNextRandomTick = 40;
    }
    else
        panicNextRandomTick--;

    baseScript->MoveToDestination(panicSpeed);
}

void AIPanicMove::panicMoveUpdateDestination()
{
    baseScript->GenerateDestination(10);
}

bool AIPanicMove::panicMoveIsPanic()
{
    return panic;
}

} /* namespace Scripting */
