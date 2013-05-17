#ifndef DATABASE_H_
#define DATABASE_H_

#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>

namespace TableBlock
{
enum eDatabaseField
{
    blockId = 1,
    name,
    lightOpacity,
    lightValue,
    blockResistance,
    blockHardness,
    needsRandomTick,
    slipperiness,
    isCollidable,
    isOpaqueCube,
    renderAsNormal,
    minX,
    minY,
    minZ,
    maxX,
    maxY,
    maxZ,
    material,
    scriptId,
    soundId,
    useNeighborBrightness,
};
}
namespace TableBlockSound
{
enum eDatabaseField
{
    soundId = 1,
    name,
    volume,
    modifier,
    stepSound,
    placeSound,
    breakSound,
};
}
namespace TableBlockMaterial
{
enum eDatabaseField
{
    materialId = 1,
    name,
    canBurn,
    replacable,
    translucent,
    requiresNoTool,
    mobilityFlag,
    solid,
    liquid,
};
}
namespace TableFurnaceRecipe
{
enum eDatabaseField
{
    itemId = 1,
    itemData,
    resultId,
    resultData,
    resultQuantity,
    experience,
};
}
namespace TableItems
{
enum eDatabaseField
{
    itemId = 1,
    name,
    maxStackSize,
    maxDamage,
    hasSubType,
    containerId,
    burningTime,
    script,
};
}

namespace TablePainting
{
enum eDatabaseField
{
    name = 1,
    sizeX,
    sizeY,
    itemId,
    itemData,
};
}

namespace TableScript
{
enum eDatabaseField
{
    id = 1,
    scriptName,
    paramCount,
};
}
namespace TableScriptData
{
enum eDatabaseField
{
    scriptId = 1,
    stuffId,
    param,
    valueInt,
    valuefloat,
    valueStr,
};
}
namespace TableScriptInfo
{
enum eDatabaseField
{
    scriptId = 1,
    name,
    paramId,
    type,
};
}

namespace TableScriptData_U_ScriptInfo
{
enum eDatabaseField
{
    param = 1,
    valueInt,
    valuefloat,
    valueStr,
    type,
};
}

namespace TableWindow
{
enum eDatabaseField
{
    id = 1,
    name,
    maxSlot,
    script,
    clientWindowId,
};
}

#endif /* DATABASE_H_ */
