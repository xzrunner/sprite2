#ifndef _SPRITE2_ACTOR_DEFAULT_H_
#define _SPRITE2_ACTOR_DEFAULT_H_

#include <CU_Singleton.h>

namespace s2
{

class ActorGeo;

class ActorDefault
{
public:	
	ActorGeo* Geo() { return m_geo; }

private:
	ActorGeo* m_geo;

	SINGLETON_DECLARATION(ActorDefault)

}; // ActorDefault

}

#endif // _SPRITE2_ACTOR_DEFAULT_H_