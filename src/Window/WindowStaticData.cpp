#include "WindowStaticData.h"

namespace Window
{

WindowStaticData::WindowStaticData(i_windowDataId id, char maxSlot, std::wstring name, int clientWindowId, Scripting::WindowScript* script)
    : id(id)
    , maxSlot(maxSlot)
    , name(name)
    , clientWindowId(clientWindowId)
    , script(script)
{
}

WindowStaticData::~WindowStaticData()
{
}

i_windowDataId WindowStaticData::getId() const
{
    return id;
}

char WindowStaticData::getMaxSlot() const
{
    return maxSlot;
}

const std::wstring& WindowStaticData::getName() const
{
    return name;
}

const Scripting::WindowScript* WindowStaticData::GetScript() const
{
    return script;
}

unsigned char WindowStaticData::GetClientWindowId() const
{
    return clientWindowId;
}

} /* namespace Window */
