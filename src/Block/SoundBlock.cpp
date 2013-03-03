#include "SoundBlock.h"

namespace Block
{

SoundBlock::SoundBlock(std::wstring& baseSoundName, float volume, float modifier)
: volume(volume)
, modifier(modifier)
, stepSound(L"step."+baseSoundName)
, placeSound(L"dig."+baseSoundName)
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
