#include "WindowStaticData.h"

#include "Window/Scripts/WindowScript.h"

namespace Window
{

WindowStaticData::WindowStaticData(i_windowDataId id, char maxSlot, char networkMaxSlot, std::wstring name, int clientWindowId, Scripting::WindowScript* script)
    : id(id)
    , maxSlot(maxSlot)
    , networkMaxSlot(networkMaxSlot)
    , name(name)
    , clientWindowId(clientWindowId)
    , script(script)
{
}

WindowStaticData::~WindowStaticData()
{
    delete script;
}

i_windowDataId WindowStaticData::getId() const
{
    return id;
}

char WindowStaticData::getNetworkMaxSlot() const
{
    return networkMaxSlot;
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
