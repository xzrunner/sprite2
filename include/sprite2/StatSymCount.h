#ifndef _SPRITE2_STAT_SYM_COUNT_H_
#define _SPRITE2_STAT_SYM_COUNT_H_

#include "StatSymType.h"

#include <cu/cu_macro.h>

#include <string>

namespace s2
{

class StatSymCount
{
public:
	void Print(std::string& str) const;

	void Add(StatSymType type);
	void Subtract(StatSymType type);

private:
	int m_counts[STAT_SYM_MAX];

	CU_SINGLETON_DECLARATION(StatSymCount);

}; // StatSymCount

}

#endif // _SPRITE2_STAT_SYM_COUNT_H_