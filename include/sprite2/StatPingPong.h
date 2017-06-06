#ifndef _SPRITE2_STAT_PINGPONG_H_
#define _SPRITE2_STAT_PINGPONG_H_

#include <CU_Singleton.h>

#include <string>

namespace s2
{

class StatPingPong
{
public:
	void Print(std::string& str) const;
	void Reset();

	void AddMesh() { ++m_mesh; }
	void AddMask() { ++m_mask; }
	void AddBlend() { ++m_blend; }
	void AddGaussianBlur() { ++m_gaussian_blur; }
	void AddOuterGlow() { ++m_outer_glow; }
	void AddDownSample() { ++m_down_sample; }
	void AddRTOutside() { ++m_rt_outside; }

private:
	int m_mesh;
	int m_mask;
	int m_blend;
	int m_gaussian_blur;
	int m_outer_glow;
	int m_down_sample;
	int m_rt_outside;

	SINGLETON_DECLARATION(StatPingPong);

}; // StatPingPong

}

#endif // _SPRITE2_STAT_PINGPONG_H_