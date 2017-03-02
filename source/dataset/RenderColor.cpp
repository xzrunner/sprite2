#include "RenderColor.h"

#include <string.h>

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

static const float INV_255 = 1.0f / 255;

static inline 
void to_abgr(uint32_t abgr, uint8_t& a, uint8_t& b, uint8_t& g, uint8_t& r)
{
	a = (abgr >> 24) & 0xff;
	b = (abgr >> 16) & 0xff;
	g = (abgr >>  8) & 0xff;
	r = abgr         & 0xff;
}

static inline 
uint32_t from_abgr(uint8_t a, uint8_t b, uint8_t g, uint8_t r)
{
	return (a << 24) | (b << 16) | (g << 8) | r;
}

RenderColor RenderColor::operator * (const RenderColor& rc) const
{
	RenderColor ret;
	// MUL
	uint32_t lmul = m_state.colors[IDX_MUL],
		     rmul = rc.m_state.colors[IDX_MUL];
	if (lmul == 0xffffffff) {
		ret.m_state.colors[IDX_MUL] = rmul;
	} else if (rmul == 0xffffffff) {
		ret.m_state.colors[IDX_MUL] = lmul;
	} else if (lmul == 0 || rmul == 0) {
		ret.m_state.colors[IDX_MUL] = 0;
	} else {
		uint8_t lr, lg, lb, la,
			    rr, rg, rb, ra;
		to_abgr(lmul, la, lb, lg, lr);
		to_abgr(rmul, ra, rb, rg, rr);
		uint8_t r, g, b, a;
		r = lr * rr * INV_255;
		g = lg * rg * INV_255;
		b = lb * rb * INV_255;
		a = la * ra * INV_255;
		ret.m_state.colors[IDX_MUL] = from_abgr(a, b, g, r);
	}
	// ADD
	uint32_t ladd = m_state.colors[IDX_ADD],
		     radd = rc.m_state.colors[IDX_ADD];
	if (ladd == 0) {
		ret.m_state.colors[IDX_ADD] = radd;
	} else if (radd == 0) {
		ret.m_state.colors[IDX_ADD] = ladd;
	} else {
		uint8_t lr, lg, lb, la,
			    rr, rg, rb, ra;
		to_abgr(ladd, la, lb, lg, lr);
		to_abgr(radd, ra, rb, rg, rr);
		uint8_t r, g, b, a;
		r = clamp(lr + rr);
		g = clamp(lg + rg);
		b = clamp(lb + rb);
		a = clamp(la + ra);
		ret.m_state.colors[IDX_ADD] = from_abgr(a, b, g, r);
	}
	// MAP
	uint32_t lrmap = m_state.colors[IDX_RMAP],
		     lgmap = m_state.colors[IDX_GMAP],
			 lbmap = m_state.colors[IDX_BMAP];
	uint32_t rrmap = rc.m_state.colors[IDX_RMAP],
		     rgmap = rc.m_state.colors[IDX_GMAP],
			 rbmap = rc.m_state.colors[IDX_BMAP];
	if (lrmap == 0x000000ff && lgmap == 0x0000ff00 && lbmap == 0x00ff0000) {
		ret.m_state.colors[IDX_RMAP] = rrmap;
		ret.m_state.colors[IDX_GMAP] = rgmap;
		ret.m_state.colors[IDX_BMAP] = rbmap;
	} else if (rrmap == 0x000000ff && rgmap == 0x0000ff00 && rbmap == 0x00ff0000) {
		ret.m_state.colors[IDX_RMAP] = lrmap;
		ret.m_state.colors[IDX_GMAP] = lgmap;
		ret.m_state.colors[IDX_BMAP] = lbmap;
	} else {
		uint8_t lrmapr, lrmapg, lrmapb, lrmapa,
				lgmapr, lgmapg, lgmapb, lgmapa,
				lbmapr, lbmapg, lbmapb, lbmapa;
		to_abgr(lrmap, lrmapa, lrmapb, lrmapg, lrmapr);
		to_abgr(lgmap, lgmapa, lgmapb, lgmapg, lgmapr);
		to_abgr(lbmap, lbmapa, lbmapb, lbmapg, lbmapr);
		uint8_t rrmapr, rrmapg, rrmapb, rrmapa,
				rgmapr, rgmapg, rgmapb, rgmapa,
				rbmapr, rbmapg, rbmapb, rbmapa;
		to_abgr(rrmap, rrmapa, rrmapb, rrmapg, rrmapr);
		to_abgr(rgmap, rgmapa, rgmapb, rgmapg, rgmapr);
		to_abgr(rbmap, rbmapa, rbmapb, rbmapg, rbmapr);

		uint8_t r, g, b;
		r = (lrmapr * rrmapr + lrmapg * rgmapr + lrmapb * rbmapr) * INV_255;
		g = (lrmapr * rrmapg + lrmapg * rgmapg + lrmapb * rbmapg) * INV_255;
		b = (lrmapr * rrmapb + lrmapg * rgmapb + lrmapb * rbmapb) * INV_255;
		ret.m_state.colors[IDX_RMAP] = from_abgr(0, b, g, r);

		r = (lgmapr * rrmapr + lgmapg * rgmapr + lgmapb * rbmapr) * INV_255;
		g = (lgmapr * rrmapg + lgmapg * rgmapg + lgmapb * rbmapg) * INV_255;
		b = (lgmapr * rrmapb + lgmapg * rgmapb + lgmapb * rbmapb) * INV_255;
		ret.m_state.colors[IDX_GMAP] = from_abgr(0, b, g, r);

		r = (lbmapr * rrmapr + lbmapg * rgmapr + lbmapb * rbmapr) * INV_255;
		g = (lbmapr * rrmapg + lbmapg * rgmapg + lbmapb * rbmapg) * INV_255;
		b = (lbmapr * rrmapb + lbmapg * rgmapb + lbmapb * rbmapb) * INV_255;
		ret.m_state.colors[IDX_BMAP] = from_abgr(0, b, g, r);
	}
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