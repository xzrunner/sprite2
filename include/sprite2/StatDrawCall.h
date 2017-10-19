#ifndef _SPRITE2_STAT_DRAWCALL_H_
#define _SPRITE2_STAT_DRAWCALL_H_

#include <cu/cu_macro.h>
#include <cu/cu_stl.h>

namespace s2
{

class StatDrawCall
{
public:
	void Print(CU_STR& str) const;
	void Reset();

	void AddScissor() { ++m_scissor; }	

private:
	int m_scissor;

	CU_SINGLETON_DECLARATION(StatDrawCall);

}; // StatDrawCall

}

#endif // _SPRITE2_STAT_DRAWCALL_H_