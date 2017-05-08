#include "S2_Symbol.h"
#include "S2_Actor.h"

#include "S2_Sprite.h"

#ifdef S2_RES_LOG
#include <iostream>
#endif // S2_RES_LOG

namespace s2
{

#ifdef S2_RES_LOG
static int COUNT = 0;
#endif // S2_RES_LOG

Symbol::Symbol() 
	: m_id(-1) 
{
#ifdef S2_RES_LOG
	++COUNT;
	std::cout << "++ symbol " << COUNT << "\n";
#endif // S2_RES_LOG
}

Symbol::Symbol(uint32_t id) 
	: m_id(id) 
{
#ifdef S2_RES_LOG
	++COUNT;
	std::cout << "++ symbol id:" << m_id << ", count: " << COUNT << "\n";
#endif // S2_RES_LOG
}

Symbol::~Symbol() 
{
#ifdef S2_RES_LOG
	--COUNT;
	std::cout << "-- symbol id:" << m_id << ", count: " << COUNT << "\n";
#endif // S2_RES_LOG
}

sm::rect Symbol::GetBounding(const Sprite* spr, const Actor* actor, bool cache) const
{	
	if (cache && actor) {
		const sm::rect& rect = actor->GetAABB().GetRect();
		return rect;
	}
	return GetBoundingImpl(spr, actor, cache);
}

}