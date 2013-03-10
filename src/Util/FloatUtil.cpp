#include "FloatUtil.h"
#include <stdlib.h>

namespace Util
{
float randFloat()
{
    return (float) rand() / (float) RAND_MAX;
}
float randFloat(float max)
{
    return (float) rand() / ((float) RAND_MAX / max);
}
float randFloat(float min, float max)
{
    return min + (float) rand() / ((float) RAND_MAX / (max - min));
}

} /* namespace World */
