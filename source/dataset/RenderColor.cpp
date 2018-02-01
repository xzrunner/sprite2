#include "sprite2/RenderColor.h"

#include <string.h>

namespace s2
{

RenderColor::RenderColor()
{
	m_colors[IDX_MUL] = 0xffffffff;
	m_colors[IDX_ADD] = 0;
	
	m_colors[IDX_RMAP] = 0x000000ff;
	m_colors[IDX_GMAP] = 0x0000ff00;
	m_colors[IDX_BMAP] = 0x00ff0000;
}

RenderColor::RenderColor(const RenderColor& col)
{
	*this = col;
}

RenderColor& RenderColor::operator = (const RenderColor& col)
{
	memcpy(m_colors, col.m_colors, sizeof(m_colors));
	return *this;
}

bool RenderColor::operator == (const RenderColor& col) const
{
	return memcmp(m_colors, col.m_colors, sizeof(m_colors)) == 0;
}

bool RenderColor::operator != (const RenderColor& col) const
{
	return !(this->operator == (col));
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
	Mul(*this, rc, ret);
	return ret;
}

void RenderColor::Reset()
{
	m_colors[IDX_MUL] = 0xffffffff;
	m_colors[IDX_ADD] = 0;

	m_colors[IDX_RMAP] = 0x000000ff;
	m_colors[IDX_GMAP] = 0x0000ff00;
	m_colors[IDX_BMAP] = 0x00ff0000;
}

void RenderColor::Mul(const RenderColor& _c0, const RenderColor& _c1, RenderColor& _c)
{
	auto& c0(_c0.m_colors);
	auto& c1(_c1.m_colors);
	auto& c(_c.m_colors);

	// MUL
	uint32_t lmul = c0[IDX_MUL],
		     rmul = c1[IDX_MUL];
	if (lmul == 0xffffffff) {
		c[IDX_MUL] = rmul;
	} else if (rmul == 0xffffffff) {
		c[IDX_MUL] = lmul;
	} else if (lmul == 0 || rmul == 0) {
		c[IDX_MUL] = 0;
	} else {
		uint8_t lr, lg, lb, la,
			    rr, rg, rb, ra;
		to_abgr(lmul, la, lb, lg, lr);
		to_abgr(rmul, ra, rb, rg, rr);
		uint8_t r, g, b, a;
		r = static_cast<uint8_t>(lr * rr * INV_255);
		g = static_cast<uint8_t>(lg * rg * INV_255);
		b = static_cast<uint8_t>(lb * rb * INV_255);
		a = static_cast<uint8_t>(la * ra * INV_255);
		c[IDX_MUL] = from_abgr(a, b, g, r);
	}
	// ADD
	uint32_t ladd = c0[IDX_ADD],
		     radd = c1[IDX_ADD];
	if (ladd == 0) {
		c[IDX_ADD] = radd;
	} else if (radd == 0) {
		c[IDX_ADD] = ladd;
	} else {
		uint8_t lr, lg, lb, la,
			    rr, rg, rb, ra;
		to_abgr(ladd, la, lb, lg, lr);
		to_abgr(radd, ra, rb, rg, rr);
		uint8_t r, g, b, a;
		r = pt2::clamp(lr + rr);
		g = pt2::clamp(lg + rg);
		b = pt2::clamp(lb + rb);
		a = pt2::clamp(la + ra);
		c[IDX_ADD] = from_abgr(a, b, g, r);
	}
	// MAP
	uint32_t lrmap = c0[IDX_RMAP],
		     lgmap = c0[IDX_GMAP],
			 lbmap = c0[IDX_BMAP];
	uint32_t rrmap = c1[IDX_RMAP],
		     rgmap = c1[IDX_GMAP],
			 rbmap = c1[IDX_BMAP];
	if (lrmap == 0x000000ff && lgmap == 0x0000ff00 && lbmap == 0x00ff0000) {
		c[IDX_RMAP] = rrmap;
		c[IDX_GMAP] = rgmap;
		c[IDX_BMAP] = rbmap;
	} else if (rrmap == 0x000000ff && rgmap == 0x0000ff00 && rbmap == 0x00ff0000) {
		c[IDX_RMAP] = lrmap;
		c[IDX_GMAP] = lgmap;
		c[IDX_BMAP] = lbmap;
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
		r = static_cast<uint8_t>((lrmapr * rrmapr + lrmapg * rgmapr + lrmapb * rbmapr) * INV_255);
		g = static_cast<uint8_t>((lrmapr * rrmapg + lrmapg * rgmapg + lrmapb * rbmapg) * INV_255);
		b = static_cast<uint8_t>((lrmapr * rrmapb + lrmapg * rgmapb + lrmapb * rbmapb) * INV_255);
		c[IDX_RMAP] = from_abgr(0, b, g, r);

		r = static_cast<uint8_t>((lgmapr * rrmapr + lgmapg * rgmapr + lgmapb * rbmapr) * INV_255);
		g = static_cast<uint8_t>((lgmapr * rrmapg + lgmapg * rgmapg + lgmapb * rbmapg) * INV_255);
		b = static_cast<uint8_t>((lgmapr * rrmapb + lgmapg * rgmapb + lgmapb * rbmapb) * INV_255);
		c[IDX_GMAP] = from_abgr(0, b, g, r);

		r = static_cast<uint8_t>((lbmapr * rrmapr + lbmapg * rgmapr + lbmapb * rbmapr) * INV_255);
		g = static_cast<uint8_t>((lbmapr * rrmapg + lbmapg * rgmapg + lbmapb * rbmapg) * INV_255);
		b = static_cast<uint8_t>((lbmapr * rrmapb + lbmapg * rgmapb + lbmapb * rbmapb) * INV_255);
		c[IDX_BMAP] = from_abgr(0, b, g, r);
	}
}

}