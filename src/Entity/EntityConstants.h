#ifndef ENTITYCONSTANTS_H_
#define ENTITYCONSTANTS_H_

namespace World
{
enum eEntityType // Use mask ? or System with type/subtype ?
{
    ENTITY_TYPE_NONE,
    ENTITY_TYPE_PLAYER,
    ENTITY_TYPE_ITEM,
    ENTITY_TYPE_FALLINGBLOCK,
    ENTITY_TYPE_HANGINGFRAME,
    ENTITY_TYPE_PAINTING,
};

}

#endif /* ENTITYCONSTANTS_H_ */
