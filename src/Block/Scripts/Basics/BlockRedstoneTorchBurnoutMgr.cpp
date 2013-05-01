#include "BlockRedstoneTorchBurnoutMgr.h"

namespace Scripting
{

BlockRedstoneTorchBurnoutMgr::BlockRedstoneTorchBurnoutMgr()
    : current_tick(0), torchHistory(128)
{
}

BlockRedstoneTorchBurnoutMgr::~BlockRedstoneTorchBurnoutMgr()
{
}

void BlockRedstoneTorchBurnoutMgr::UpdateTick()
{
    current_tick++;

    while (!torchHistory.empty())
    {
        TorchData& front = torchHistory.front();
        if (front.tick < current_tick - 60)
        {
            torchHistory.pop_front();
        }
        else
        {
            break;
        }
    }
}

bool BlockRedstoneTorchBurnoutMgr::CheckTorchBurnout(int x, i_height y, int z)
{
    unsigned char count = 0;
    size_t size = torchHistory.size();
    for (size_t i = 0; i < size; i++)
    {
        TorchData& data = torchHistory[i];
        if (data.y == y && data.x == x && data.z == z)
        {
            count++;
            if (count >= 8)
                return true;
        }
    }
    return false;
}

void BlockRedstoneTorchBurnoutMgr::ChangeTorchState(int x, i_height y, int z)
{
    if (torchHistory.full())
        torchHistory.rset_capacity(torchHistory.capacity() * 2);
    torchHistory.push_back({ x, y, z, current_tick });
}

} /* namespace Scripting */
