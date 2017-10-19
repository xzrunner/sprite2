#ifndef _SPRITE2_STAT_PINGPONG_H_
#define _SPRITE2_STAT_PINGPONG_H_

#include <cu/cu_macro.h>
#include <cu/cu_stl.h>

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
	void Print(CU_STR& str) const;
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

	CU_SINGLETON_DECLARATION(StatPingPong);

}; // StatPingPong

}

#endif // _SPRITE2_STAT_PINGPONG_H_