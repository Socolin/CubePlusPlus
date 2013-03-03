#ifndef SOUNDBLOCK_H_
#define SOUNDBLOCK_H_

#include <string>

namespace Block
{

class SoundBlock
{
public:
    SoundBlock(std::wstring& breakSound, std::wstring& stepSound, std::wstring& placeSound, float volume, float modifier);
    SoundBlock();
    virtual ~SoundBlock();

    inline float GetVolume() const { return volume; }
    inline float GetModifier() const { return modifier; }
    inline const std::wstring& GetStepSound() const { return stepSound; }
    inline const std::wstring& GetBreakSound() const { return placeSound; } // TODO: change this
    inline const std::wstring& GetPlaceSound() const { return placeSound; }
private:
    float volume;
    float modifier;
    std::wstring stepSound;
    std::wstring placeSound;
};

} /* namespace Inventory */
#endif /* SOUNDBLOCK_H_ */
