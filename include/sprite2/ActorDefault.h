#ifndef _SPRITE2_ACTOR_DEFAULT_H_
#define _SPRITE2_ACTOR_DEFAULT_H_

#include "sprite2/CompActorTrans.h"

#include <cu/cu_macro.h>

#include <memory>

namespace s2
{

class ActorDefault
{
public:	
	const CompActorTrans& Transform() const { return *m_transform; }

private:
	std::unique_ptr<CompActorTrans> m_transform;

	CU_SINGLETON_DECLARATION(ActorDefault)

}; // ActorDefault

}

#endif // _SPRITE2_ACTOR_DEFAULT_H_