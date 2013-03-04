#include "SoundBlock.h"

namespace Block
{

SoundBlock::SoundBlock(std::wstring& breakSound, std::wstring& stepSound, std::wstring& placeSound, float volume, float modifier)
    : volume(volume)
    , modifier(modifier)
    , stepSound(stepSound)
    , placeSound(placeSound)
{
}

SoundBlock::SoundBlock()
    : volume(1)
    , modifier(2)
    , stepSound(L"step.stone")
    , placeSound(L"dig.stone")
{
}

SoundBlock::~SoundBlock()
{
}

} /* namespace Inventory */
