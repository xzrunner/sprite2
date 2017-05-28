#ifndef _SPRITE2_STATISTICS_H_
#define _SPRITE2_STATISTICS_H_

#include <CU_Singleton.h>

namespace s2
{

class Statistics
{
public:
	void AddOverdrawArea(float area) { m_overdraw_area += area; }

	float GetOverdrawArea() const { return m_overdraw_area; }
	
	void Reset();

private:
	float m_overdraw_area;

	SINGLETON_DECLARATION(Statistics);

}; // Statistics

}

#endif // _SPRITE2_STATISTICS_H_