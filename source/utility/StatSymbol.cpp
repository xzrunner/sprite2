#include "StatSymbol.h"

#include <glp_loop.h>

namespace s2
{

SINGLETON_DEFINITION(StatSymbol);

StatSymbol::StatSymbol()
{
}

void StatSymbol::Print(std::string& str) const
{
	m_items[SYM_IMAGE].Print(str, "img");
	m_items[SYM_SCALE9].Print(str, "s9");
	m_items[SYM_ICON].Print(str, "icon");
	m_items[SYM_TEXTURE].Print(str, "tex");
	m_items[SYM_TEXTBOX].Print(str, "text");
	m_items[SYM_COMPLEX].Print(str, "comp");
	m_items[SYM_ANIMATION].Print(str, "anim");
	m_items[SYM_ANIM2].Print(str, "anim2");
	m_items[SYM_PARTICLE3D].Print(str, "p3d");
	m_items[SYM_PARTICLE2D].Print(str, "p2d");
	m_items[SYM_SHAPE].Print(str, "shape");
	m_items[SYM_MESH].Print(str, "mesh");
	m_items[SYM_MASK].Print(str, "mask");
	m_items[SYM_TRAIL].Print(str, "trail");
	m_items[SYM_SKELETON].Print(str, "sk");
}

void StatSymbol::Reset()
{
	for (int i = 0; i < SYM_MAX; ++i) {
		m_items[i].Reset();
	}
}

void StatSymbol::AddDrawCount(StatSymbol::Type type)
{
	++m_items[type].m_draw_count;
}

void StatSymbol::AddDrawCost(StatSymbol::Type type, uint32_t cost)
{
	m_items[type].m_draw_cost += cost;	
}

/************************************************************************/
/* class StatSymbol::DrawCostCP                                         */
/************************************************************************/

StatSymbol::DrawCostCP::DrawCostCP(Type type)
	: m_type(type)
{
	m_begin = glp_get_time();
}

StatSymbol::DrawCostCP::~DrawCostCP()
{
	uint32_t cost = glp_get_time() - m_begin;
	StatSymbol::Instance()->AddDrawCost(m_type, cost);
}

/************************************************************************/
/* class StatSymbol::Item                                               */
/************************************************************************/

StatSymbol::Item::Item() 
	: m_draw_count(0)
	, m_draw_cost(0) 
{
}

void StatSymbol::Item::Print(std::string& str, const std::string& tag) const
{
	static char buf[512];
	sprintf(buf, "%s: count %d, cost %d\n", tag.c_str(), m_draw_count, m_draw_cost);
	str += buf;
}

void StatSymbol::Item::Reset() 
{ 
	m_draw_count = 0; 
	m_draw_cost = 0; 
}

}