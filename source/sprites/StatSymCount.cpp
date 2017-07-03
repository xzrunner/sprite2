#include "StatSymCount.h"

#include <stdio.h>
#include <string.h>

namespace s2
{

SINGLETON_DEFINITION(StatSymCount);

StatSymCount::StatSymCount()
{
	memset(m_counts, 0, sizeof(m_counts));
}

void StatSymCount::Print(std::string& str) const
{
	static char buf[512];
	sprintf(buf, "SYM: img %d, complex %d, anim %d\n", 
		m_counts[STAT_SYM_IMAGE], m_counts[STAT_SYM_COMPLEX], m_counts[STAT_SYM_ANIMATION]);
	str += buf;
}

void StatSymCount::Add(StatSymType type)
{
	++m_counts[type];
}

void StatSymCount::Subtract(StatSymType type)
{
	--m_counts[type];
}

}