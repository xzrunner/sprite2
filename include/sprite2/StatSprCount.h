#ifndef _SPRITE2_STAT_SPR_COUNT_H_
#define _SPRITE2_STAT_SPR_COUNT_H_

#include "StatSymType.h"

#include <cu/cu_macro.h>
#include <cu/cu_stl.h>

namespace s2
{

class StatSprCount
{
public:
	void Print(CU_STR& str) const;
	void Reset();

	void Add(StatSymType type);
	void Subtract(StatSymType type);

private:
	int m_counts[STAT_SYM_MAX];

	CU_SINGLETON_DECLARATION(StatSprCount);

}; // StatSprCount

}

#endif // _SPRITE2_STAT_SPR_COUNT_H_