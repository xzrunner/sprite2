#include "StatSprCount.h"

#include <stdio.h>
#include <string.h>

namespace s2
{

CU_SINGLETON_DEFINITION(StatSprCount);

StatSprCount::StatSprCount()
{
	memset(m_counts, 0, sizeof(m_counts));
}

void StatSprCount::Print(CU_STR& str) const
{
	static char buf[512];
	sprintf(buf, "SPR: img %d, complex %d, anim %d\n", 
		m_counts[STAT_SYM_IMAGE], m_counts[STAT_SYM_COMPLEX], m_counts[STAT_SYM_ANIMATION]);
	str += buf;
}

void StatSprCount::Reset()
{
}

void StatSprCount::Add(StatSymType type)
{
	++m_counts[type];
}

void StatSprCount::Subtract(StatSymType type)
{
	--m_counts[type];
}

}