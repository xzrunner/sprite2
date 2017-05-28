#include "sprite2/Statistics.h"

#include <string.h>

namespace s2
{

SINGLETON_DEFINITION(Statistics);

Statistics::Statistics() 
	: m_overdraw_area(0) 
{}

void Statistics::Reset()
{
	m_overdraw_area = 0;
	m_pp_count.Reset();
}

/************************************************************************/
/* class Statistics::PingPongCount                                      */
/************************************************************************/

Statistics::PingPongCount::PingPongCount() 
{
	Reset();
}

void Statistics::PingPongCount::Reset() 
{
	memset(this, 0, sizeof(*this));
}

/************************************************************************/
/* class Statistics::DrawCallCount                                      */
/************************************************************************/

Statistics::DrawCallCount::DrawCallCount() 
{
	Reset();
}

void Statistics::DrawCallCount::Reset() 
{
	memset(this, 0, sizeof(*this));
}

}