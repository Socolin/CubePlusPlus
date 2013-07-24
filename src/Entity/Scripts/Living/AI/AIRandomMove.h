#ifndef AIRANDOMMOVE_H_
#define AIRANDOMMOVE_H_

namespace Scripting
{

class LivingEntityScript;
class AIRandomMove
{
public:
    AIRandomMove();
    virtual ~AIRandomMove();

protected:
    void randomMoveInit(LivingEntityScript* script);
    void randomMoveUpdate();
    void randomMoveUpdateDestination();
private:

    // Random move module
    int randomMoveTimer;
    bool notMoving;
    float randomMoveSpeed;
private:
    LivingEntityScript* baseScript;
};

} /* namespace Scripting */


#endif /* AIRANDOMMOVE_H_ */
