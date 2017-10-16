#ifndef _SPRITE2_ACTOR_DEFAULT_H_
#define _SPRITE2_ACTOR_DEFAULT_H_

#include <cu/cu_macro.h>

namespace s2
{

class ActorGeo;

class ActorDefault
{
public:	
	auto& Geo() { return m_geo; }

private:
	ActorGeo* m_geo;

	CU_SINGLETON_DECLARATION(ActorDefault)

}; // ActorDefault

}

#endif // _SPRITE2_ACTOR_DEFAULT_H_