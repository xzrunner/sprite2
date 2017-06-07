#include "StatPingPong.h"

#include <string.h>

namespace s2
{

SINGLETON_DEFINITION(StatPingPong);

StatPingPong::StatPingPong()
{
	Reset();
}

void StatPingPong::Print(std::string& str) const
{
	static char buf[512];
	sprintf(buf, "PingPong: mesh %d, mask %d, blend %d, \nblur %d, glow %d, downsample %d, out %d", 
		m_mesh, m_mask, m_blend, m_gaussian_blur, m_outer_glow, m_down_sample, m_rt_outside);
	str += buf;
}

void StatPingPong::Reset()
{
	memset(this, 0, sizeof(*this));
}

}