#ifndef AIPANICMOVE_H_
#define AIPANICMOVE_H_

namespace Scripting
{
class LivingEntityScript;
class AIPanicMove
{
protected:
    AIPanicMove();
    virtual ~AIPanicMove();

    void panicMoveInit(LivingEntityScript* script, float speed);
    void panicMoveStart();
    void panicMoveUpdate();
    void panicMoveUpdateDestination();
    bool panicMoveIsPanic();
private:
    bool panic;
    int panicTimer;
    float panicSpeed;
    int panicNextRandomTick;

    LivingEntityScript* baseScript;
};

} /* namespace Scripting */

#endif /* AIPANICMOVE_H_ */

