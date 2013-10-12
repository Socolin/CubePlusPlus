#ifndef COLORUTIL_H_
#define COLORUTIL_H_

#include "Util/Singleton.h"
#include <map>

namespace Util{
class ColorUtil : public Singleton<ColorUtil>
{
    friend class Singleton<ColorUtil>;
public:
    virtual ~ColorUtil();
    char GetMatchingRecipe(char color1, char color2);
private:
    void init();
    ColorUtil();
    std::map<std::pair<char,char>,char> matchingRecipe;
};
} /* namespace Util */
#endif /* COLORUTIL_H_ */
