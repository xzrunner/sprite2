#ifndef _SPRITE2_STAT_DRAWCALL_H_
#define _SPRITE2_STAT_DRAWCALL_H_

#include <CU_Singleton.h>

#include <string>

namespace s2
{

class StatDrawCall
{
public:
	void Print(std::string& str) const;
	void Reset();

	void AddScissor() { ++m_scissor; }	

private:
	int m_scissor;

	SINGLETON_DECLARATION(StatDrawCall);

}; // StatDrawCall

}

#endif // _SPRITE2_STAT_DRAWCALL_H_