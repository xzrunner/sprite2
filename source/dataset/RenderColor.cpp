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
	m_state.mul = col.m_state.mul;
	m_state.add = col.m_state.add;
	m_state.rmap = col.m_state.rmap;
	m_state.gmap = col.m_state.gmap;
	m_state.bmap = col.m_state.bmap;
	return *this;
}

RenderColor RenderColor::operator * (const RenderColor& rc) const
{
	RenderColor ret;
	ret.m_state.mul  = m_state.mul * rc.m_state.mul;
	ret.m_state.add  = m_state.add + rc.m_state.add;
	ret.m_state.rmap = m_state.rmap.MapMul(rc.m_state.rmap, rc.m_state.gmap, rc.m_state.bmap);
	ret.m_state.gmap = m_state.gmap.MapMul(rc.m_state.rmap, rc.m_state.gmap, rc.m_state.bmap);
	ret.m_state.bmap = m_state.bmap.MapMul(rc.m_state.rmap, rc.m_state.gmap, rc.m_state.bmap);
	return ret;
}

void RenderColor::Init()
{
	m_state.mul.FromRGBA(0xffffffff);
	m_state.add.FromRGBA(0);

	m_state.rmap.FromRGBA(0xff000000);
	m_state.gmap.FromRGBA(0x00ff0000);
	m_state.bmap.FromRGBA(0x0000ff00);
}

}