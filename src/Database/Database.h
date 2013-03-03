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
    material,
    scriptId,
    soundId,
};
}
#endif /* DATABASE_H_ */
