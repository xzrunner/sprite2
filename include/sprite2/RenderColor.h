#ifndef _SPRITE2_RENDER_COLOR_H_
#define _SPRITE2_RENDER_COLOR_H_

#include "Color.h"

namespace s2
{

class RenderColor
{
public:
	RenderColor();
	RenderColor(const RenderColor& col);
	RenderColor& operator = (const RenderColor& col);

	bool operator == (const RenderColor& col) const;
	bool operator != (const RenderColor& col) const;

	RenderColor operator * (const RenderColor& rc) const;

	void Reset();

	Color GetMul() const { Color ret; ret.FromABGR(m_colors[IDX_MUL]); return ret; }
	Color GetAdd() const { Color ret; ret.FromABGR(m_colors[IDX_ADD]); return ret; }
	Color GetRMap() const { Color ret; ret.FromABGR(m_colors[IDX_RMAP]); return ret; }
	Color GetGMap() const { Color ret; ret.FromABGR(m_colors[IDX_GMAP]); return ret; }
	Color GetBMap() const { Color ret; ret.FromABGR(m_colors[IDX_BMAP]); return ret; }

	void SetMul(const Color& mul) { m_colors[IDX_MUL] = mul.ToABGR(); }
	void SetAdd(const Color& add) { m_colors[IDX_ADD] = add.ToABGR(); }
	void SetRMap(const Color& rmap) { m_colors[IDX_RMAP] = rmap.ToABGR(); }
	void SetGMap(const Color& gmap) { m_colors[IDX_GMAP] = gmap.ToABGR(); }
	void SetBMap(const Color& bmap) { m_colors[IDX_BMAP] = bmap.ToABGR(); }

	uint32_t GetMulABGR() const { return m_colors[IDX_MUL]; }
	uint32_t GetAddABGR() const { return m_colors[IDX_ADD]; }
	uint32_t GetRMapABGR() const { return m_colors[IDX_RMAP]; }
	uint32_t GetGMapABGR() const { return m_colors[IDX_GMAP]; }
	uint32_t GetBMapABGR() const { return m_colors[IDX_BMAP]; }

	void SetMulABGR(uint32_t mul) { m_colors[IDX_MUL] = mul; }
	void SetAddABGR(uint32_t add) { m_colors[IDX_ADD] = add; }
	void SetRMapABGR(uint32_t rmap) { m_colors[IDX_RMAP] = rmap; }
	void SetGMapABGR(uint32_t gmap) { m_colors[IDX_GMAP] = gmap; }
	void SetBMapABGR(uint32_t bmap) { m_colors[IDX_BMAP] = bmap; }

	static void Mul(const RenderColor& c0, const RenderColor& c1, RenderColor& c);

private:
	enum COLOR_IDX
	{
		IDX_MUL = 0,
		IDX_ADD,
		IDX_RMAP,
		IDX_GMAP,
		IDX_BMAP,

		COLOR_MAX
	};

	uint32_t m_colors[COLOR_MAX]; // in ABGR

}; // RenderColor

}

#endif // _SPRITE2_RENDER_COLOR_H_