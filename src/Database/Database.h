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
    burningTime,
    useNeighborBrightness,
    replaceBlockId,
    dropId,
    dropDataMask,
    dropCountMin,
    dropCountMax,
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
    blocksMovement,
};
}
namespace TableCraft
{
enum eDatabaseField
{
    id = 1,
    width,
    height,
    resultId,
    resultData,
    resultQuantity,
    scriptId,
};
}
namespace TableCraftSlot
{
enum eDatabaseField
{
    id = 1,
    craftId,
    itemId,
    itemData,
};
}
namespace TableEntityLiving
{
enum eDatabaseField
{
    id = 1,
    name,
    clientType,
    script,
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

namespace TableMessage
{
enum eDatabaseField
{
    id = 1,
    langId,
    key,
    text,
};
}

namespace TableMessageLang
{
enum eDatabaseField
{
    id = 1,
    lang,
    description,
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
namespace TableToolEfficiency
{
enum eDatabaseField
{
    id = 1,
    toolTypeId,
    blockId,
    materialId,
    requireHarvestLevel,
};
}
namespace TableToolMaterial
{
enum eDatabaseField
{
    id = 1,
    name,
    harvestLevel,
    maxUses,
    efficiency,
    damageVsEntity,
    enchantability,
};
}
namespace TableWindow
{
enum eDatabaseField
{
    id = 1,
    name,
    maxSlot,
    networkMaxSlot,
    script,
    clientWindowId,
};
}

#endif /* DATABASE_H_ */
