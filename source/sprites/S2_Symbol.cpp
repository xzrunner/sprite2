#include "S2_Symbol.h"
#include "S2_Actor.h"

#include "S2_Sprite.h"

#ifdef S2_RES_LOG
#include <iostream>
#endif // S2_RES_LOG

namespace s2
{

static int ALL_SYM_COUNT = 0;

Symbol::Symbol() 
	: m_id(-1) 
{
	++ALL_SYM_COUNT;
}

Symbol::Symbol(uint32_t id) 
	: m_id(id) 
{
	++ALL_SYM_COUNT;
}

Symbol::~Symbol() 
{
	--ALL_SYM_COUNT;
}

sm::rect Symbol::GetBounding(const SprConstPtr& spr, const ActorConstPtr& actor, bool cache) const
{	
	if (cache && actor) {
		const sm::rect& rect = actor->GetAABB().GetRect();
		return rect;
	}
	return GetBoundingImpl(spr, actor, cache);
}

int Symbol::GetAllSymCount()
{
	return ALL_SYM_COUNT;
}

}