#ifndef CRAFTSCRIPT_H_
#define CRAFTSCRIPT_H_

#include <string>

namespace Craft
{
class Craft;
}

namespace Scripting
{
class CraftScript
{
public:
    CraftScript(const char* scriptName);
    virtual ~CraftScript();

    virtual CraftScript* Copy() = 0;

    virtual void InitParam(int /*paramId*/, int /*param*/) {}
    virtual void InitParam(int /*paramId*/, float /*param*/) {}
    virtual void InitParam(int /*paramId*/, const std::string& /*param*/) {}

    //TODO
private:
    Craft::Craft* baseCraft;
};

} /* namespace Scripting */
#endif /* CRAFTSCRIPT_H_ */
