#include "ColorUtil.h"
#include <map>

namespace Util{
ColorUtil::ColorUtil() {
    init();
}

ColorUtil::~ColorUtil() {

}

char ColorUtil::GetMatchingRecipe(char color1, char color2){
    std::map<std::pair<char,char>,char>::const_iterator pos = matchingRecipe.find(std::make_pair(color1,color2));
    if(pos != matchingRecipe.end())
    {
        return char(pos->second);
    }
    else
    {
        return -1;
    }
}

void ColorUtil::init(){
    matchingRecipe.insert(std::make_pair(std::make_pair(0,7),8));
    matchingRecipe.insert(std::make_pair(std::make_pair(0,11),3));
    matchingRecipe.insert(std::make_pair(std::make_pair(0,13),5));
    matchingRecipe.insert(std::make_pair(std::make_pair(0,14),6));
    matchingRecipe.insert(std::make_pair(std::make_pair(0,15),7));
    matchingRecipe.insert(std::make_pair(std::make_pair(4,14),1));
    matchingRecipe.insert(std::make_pair(std::make_pair(6,10),2));
    matchingRecipe.insert(std::make_pair(std::make_pair(11,13),9));
    matchingRecipe.insert(std::make_pair(std::make_pair(11,14),10));
}
} /* namespace Util */
