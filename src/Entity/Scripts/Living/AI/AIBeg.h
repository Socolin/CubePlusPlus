/*
 * AIBeg.h
 *
 *  Created on: Aug 13, 2013
 *      Author: yann291
 */

#ifndef AIBEG_H_
#define AIBEG_H_

namespace World
{
class ScriptedLivingEntity;
}
namespace Scripting
{
class AnimalWolfScript;
class AIBeg {
public:
	AIBeg();
	virtual ~AIBeg();
protected:
	void begInit(AnimalWolfScript* script);
	void begStartBegging();
	void begUpdate(World::ScriptedLivingEntity* baseEntity);
private:
    AnimalWolfScript* wolfScript;
    int begTimer;
};

} /* namespace Scripting */
#endif /* AIBEG_H_ */
