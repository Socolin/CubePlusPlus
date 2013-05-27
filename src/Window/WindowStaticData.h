#ifndef WINDOWSTATICDATA_H_
#define WINDOWSTATICDATA_H_

#include <string>

#include "Util/types.h"

namespace Scripting
{
class WindowScript;
}
namespace Window
{

class WindowStaticData
{
public:
    WindowStaticData(i_windowDataId id, char maxSlot, char networkMaxSlot, std::wstring name, int clientWindowId, Scripting::WindowScript* script);
    virtual ~WindowStaticData();
    i_windowDataId getId() const;
    char getNetworkMaxSlot() const;
    const std::wstring& getName() const;
    const Scripting::WindowScript* GetScript() const;
    unsigned char GetClientWindowId() const;

private:
    i_windowDataId id;
    char maxSlot;// TODO: remove it
    char networkMaxSlot;
    std::wstring name;
    unsigned char clientWindowId;
    Scripting::WindowScript* script;
};

} /* namespace Window */
#endif /* WINDOWSTATICDATA_H_ */
