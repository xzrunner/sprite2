#include "sprite2/StatSymDraw.h"

#include <glp_loop.h>

#include <stdio.h>

namespace s2
{

CU_SINGLETON_DEFINITION(StatSymDraw);

StatSymDraw::StatSymDraw()
{
}

void StatSymDraw::Print(CU_STR& str) const
{
	m_items[STAT_SYM_IMAGE].Print(str, "img");
	m_items[STAT_SYM_SCALE9].Print(str, "s9");
	m_items[STAT_SYM_ICON].Print(str, "icon");
	m_items[STAT_SYM_TEXTURE].Print(str, "tex");
	m_items[STAT_SYM_TEXTBOX].Print(str, "text");
	m_items[STAT_SYM_COMPLEX].Print(str, "comp");
	m_items[STAT_SYM_ANIMATION].Print(str, "anim");
	m_items[STAT_SYM_ANIM2].Print(str, "anim2");
	m_items[STAT_SYM_PARTICLE3D].Print(str, "p3d");
	m_items[STAT_SYM_PARTICLE2D].Print(str, "p2d");
	m_items[STAT_SYM_SHAPE].Print(str, "shape");
	m_items[STAT_SYM_MESH].Print(str, "mesh");
	m_items[STAT_SYM_MASK].Print(str, "mask");
	m_items[STAT_SYM_TRAIL].Print(str, "trail");
	m_items[STAT_SYM_SKELETON].Print(str, "sk");
}

void StatSymDraw::Reset()
{
	for (int i = 0; i < STAT_SYM_MAX; ++i) {
		m_items[i].Reset();
	}
}

void StatSymDraw::AddDrawCount(StatSymType type)
{
	++m_items[type].m_draw_count;
}

void StatSymDraw::AddDrawCost(StatSymType type, uint32_t cost)
{
	m_items[type].m_draw_cost += cost;	
}

/************************************************************************/
/* class StatSymDraw::DrawCostCP                                         */
/************************************************************************/

StatSymDraw::DrawCostCP::DrawCostCP(StatSymType type)
	: m_type(type)
{
	m_begin = glp_get_time();
}

StatSymDraw::DrawCostCP::~DrawCostCP()
{
	uint32_t cost = glp_get_time() - m_begin;
	StatSymDraw::Instance()->AddDrawCost(m_type, cost);
}

/************************************************************************/
/* class StatSymDraw::Item                                               */
/************************************************************************/

StatSymDraw::Item::Item() 
	: m_draw_count(0)
	, m_draw_cost(0) 
{
}

void StatSymDraw::Item::Print(CU_STR& str, const CU_STR& tag) const
{
	static char buf[512];
	sprintf(buf, "%s: count %d, cost %d\n", tag.c_str(), m_draw_count, m_draw_cost);
	str += buf;
}

void StatSymDraw::Item::Reset() 
{ 
	m_draw_count = 0; 
	m_draw_cost = 0; 
}

}