#include "sprite2/Statistics.h"

namespace s2
{

SINGLETON_DEFINITION(Statistics);

Statistics::Statistics() 
	: m_overdraw_area(0) 
{}

void Statistics::Reset()
{
	m_overdraw_area = 0;
}

}