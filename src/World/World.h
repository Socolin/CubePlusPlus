/*
 * World.h
 *
 *  Created on: 5 déc. 2012
 *      Author: bertrand
 */

#ifndef WORLD_H_
#define WORLD_H_

#include <unordered_map>
#include <string>

namespace World
{
class Chunk;

class World
{
public:
	World();
	virtual ~World();
	void Update();
private:
	std::unordered_map<std::string, Chunk*> chunkMap;
};

} /* namespace Network */
#endif /* WORLD_H_ */
