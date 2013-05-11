#ifndef WINDOWSTATICDATA_H_
#define WINDOWSTATICDATA_H_

#include <string>

#include "Util/types.h"

namespace Window
{

class WindowStaticData
{
public:
    WindowStaticData(i_windowDataId id, int maxSlot);
    virtual ~WindowStaticData();
private:
    i_windowDataId id;
    std::string name;
    std::string scriptName;
    int maxSlot;
};

} /* namespace Window */
#endif /* WINDOWSTATICDATA_H_ */
