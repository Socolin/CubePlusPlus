#ifndef AIRANDOMMOVE_H_
#define AIRANDOMMOVE_H_

namespace Scripting
{

class LivingEntityScript;
class AIRandomMove
{
protected:
    AIRandomMove();
    virtual ~AIRandomMove();

    void randomMoveInit(LivingEntityScript* script, float speed);
    void randomMoveUpdate();
    void randomMoveUpdateDestination();
private:
    int randomMoveTimer;
    bool randomMoveNotMoving;
    float randomMoveSpeed;
    LivingEntityScript* baseScript;
};

} /* namespace Scripting */


#endif /* AIRANDOMMOVE_H_ */
