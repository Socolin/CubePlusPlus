#ifndef BLOCKREDSTONETORCHBURNOUTMGR_H_
#define BLOCKREDSTONETORCHBURNOUTMGR_H_

#include <boost/circular_buffer.hpp>

#include "Util/types.h"

namespace Scripting
{

class BlockRedstoneTorchBurnoutMgr
{
public:
    BlockRedstoneTorchBurnoutMgr();
    virtual ~BlockRedstoneTorchBurnoutMgr();

    void UpdateTick();
    void ChangeTorchState(int x, i_height y, int z);
    bool CheckTorchBurnout(int x, i_height y, int z);
private:
    struct TorchData
    {
        int x;
        i_height y;
        int z;
        unsigned tick;
    };
    unsigned current_tick;
    boost::circular_buffer<TorchData> torchHistory;
};

} /* namespace Scripting */
#endif /* BLOCKREDSTONETORCHBURNOUTMGR_H_ */
