#ifndef _SPRITE2_STAT_OVERDRAW_H_
#define _SPRITE2_STAT_OVERDRAW_H_

#include <cu/cu_macro.h>
#include <cu/cu_stl.h>

namespace s2
{

class StatOverdraw
{
public:
	void Print(CU_STR& str) const;
	void Reset();

	void AddArea(float area) { m_area += area; }

private:
	float m_area;

	CU_SINGLETON_DECLARATION(StatOverdraw);

}; // StatOverdraw

}

#endif // _SPRITE2_STAT_OVERDRAW_H_