#ifndef WINDOWLIST_H_
#define WINDOWLIST_H_

#include "Util/Singleton.h"

#include "Util/types.h"

namespace Window
{
#define MAX_WINDOW 255

class WindowStaticData;
class WindowList : public Util::Singleton<WindowList>
{
    friend class Util::Singleton<WindowList>;
public:
    static const WindowStaticData* getWindowData(i_windowDataId windowId)
    {
        return Instance().windows[windowId];
    }
    virtual ~WindowList();
    void InitInstance() override;
private:
    WindowList();
    void Initialize();
    void GenerateItemBlock();
    void LoadScripts();
public:
    WindowStaticData* windows[MAX_WINDOW];
};

} /* namespace Window */
#endif /* WINDOWLIST_H_ */
