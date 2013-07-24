#ifndef AIPANICMOVE_H_
#define AIPANICMOVE_H_

namespace Scripting
{
class LivingEntityScript;
class AIPanicMove
{
public:
    AIPanicMove();
    virtual ~AIPanicMove();
protected:
    void panicMoveInit(LivingEntityScript* script);
    void panicMoveStart();
    void panicMoveUpdate();
    void panicMoveUpdateDestination();
    bool panicMoveIsPanic();
private:
    bool panic;
    int panicTimer;
    float panicSpeed;
    int panicNextRandomTick;
private:
    LivingEntityScript* baseScript;
};

} /* namespace Scripting */

#endif /* AIPANICMOVE_H_ */

