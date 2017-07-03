#ifndef _SPRITE2_STAT_SYM_COUNT_H_
#define _SPRITE2_STAT_SYM_COUNT_H_

#include "StatSymType.h"

#include <CU_Singleton.h>

#include <string>

namespace s2
{

class StatSymCount
{
public:
	void Print(std::string& str) const;
	void Reset();

	void Add(StatSymType type);
	void Subtract(StatSymType type);

private:
	int m_counts[STAT_SYM_MAX];

	SINGLETON_DECLARATION(StatSymCount);

}; // StatSymCount

}

#endif // _SPRITE2_STAT_SYM_COUNT_H_