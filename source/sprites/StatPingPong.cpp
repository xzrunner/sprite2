#include "sprite2/StatPingPong.h"


#include <stdio.h>
#include <string.h>

namespace s2
{

CU_SINGLETON_DEFINITION(StatPingPong);

StatPingPong::StatPingPong()
{
	Reset();
}

void StatPingPong::Print(CU_STR& str) const
{
	static char buf[512];
	sprintf(buf, "PingPong: mesh %d, mask %d, blend %d, \nblur %d, glow %d, downsample %d, integrate %d, out %d", 
		m_mesh, m_mask, m_blend, m_gaussian_blur, m_outer_glow, m_down_sample, m_integrate, m_rt_outside);
	str += buf;
}

void StatPingPong::Reset()
{
	memset(this, 0, sizeof(*this));
}

void StatPingPong::AddCount(Type type)
{
	switch (type)
	{
	case MESH:
		++m_mesh;
		break;
	case MASK:
		++m_mask;
		break;
	case BLEND:
		++m_blend;
		break;
	case GAUSSIAN_BLUR:
		++m_gaussian_blur;
		break;
	case OUTER_GLOW:
		++m_outer_glow;
		break;
	case DOWN_SAMPLE:
		++m_down_sample;
		break;
	case INTEGRATE:
		++m_integrate;
		break;
	case RT_OUTSIDE:
		++m_rt_outside;
		break;
	}
}

}