#ifndef _SPRITE2_STAT_PINGPONG_H_
#define _SPRITE2_STAT_PINGPONG_H_

#include <CU_Singleton.h>

#include <string>

namespace s2
{

class StatPingPong
{
public:
	enum Type
	{
		MESH = 0,
		MASK,
		BLEND,
		GAUSSIAN_BLUR,
		OUTER_GLOW,
		DOWN_SAMPLE,
		INTEGRATE,
		RT_OUTSIDE,
	};

public:
	void Print(std::string& str) const;
	void Reset();

	void AddCount(Type type);

private:
	int m_mesh;
	int m_mask;
	int m_blend;
	int m_gaussian_blur;
	int m_outer_glow;
	int m_down_sample;
	int m_integrate;
	int m_rt_outside;

	SINGLETON_DECLARATION(StatPingPong);

}; // StatPingPong

}

#endif // _SPRITE2_STAT_PINGPONG_H_