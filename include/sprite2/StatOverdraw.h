#ifndef _SPRITE2_STAT_OVERDRAW_H_
#define _SPRITE2_STAT_OVERDRAW_H_

#include <CU_Singleton.h>

#include <string>

namespace s2
{

class StatOverdraw
{
public:
	void Print(std::string& str) const;
	void Reset();

	void AddArea(float area) { m_area += area; }

private:
	float m_area;

	SINGLETON_DECLARATION(StatOverdraw);

}; // StatOverdraw

}

#endif // _SPRITE2_STAT_OVERDRAW_H_