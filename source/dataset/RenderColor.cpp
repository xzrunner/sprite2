#include "RenderColor.h"

namespace s2
{

RenderColor::RenderColor()
{
	Init();
}

RenderColor::RenderColor(const RenderColor& col)
{
	*this = col;
}

RenderColor& RenderColor::operator = (const RenderColor& col)
{
	memcpy(m_state.colors, col.m_state.colors, sizeof(m_state.colors));
	return *this;
}

RenderColor RenderColor::operator * (const RenderColor& rc) const
{
	RenderColor ret;
	ret.m_state.colors[IDX_MUL] = (GetMul() * rc.GetMul()).ToABGR();
 	ret.m_state.colors[IDX_ADD] = (GetAdd() + rc.GetAdd()).ToABGR();
	ret.m_state.colors[IDX_RMAP] = (GetRMap().MapMul(rc.GetRMap(), rc.GetGMap(), rc.GetBMap())).ToABGR();
	ret.m_state.colors[IDX_GMAP] = (GetGMap().MapMul(rc.GetRMap(), rc.GetGMap(), rc.GetBMap())).ToABGR();
	ret.m_state.colors[IDX_BMAP] = (GetBMap().MapMul(rc.GetRMap(), rc.GetGMap(), rc.GetBMap())).ToABGR();
	return ret;
}

void RenderColor::Init()
{
	m_state.colors[IDX_MUL] = 0xffffffff;
	m_state.colors[IDX_ADD] = 0;

	m_state.colors[IDX_RMAP] = 0x000000ff;
	m_state.colors[IDX_GMAP] = 0x0000ff00;
	m_state.colors[IDX_BMAP] = 0x00ff0000;
}

}