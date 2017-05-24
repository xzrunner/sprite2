#ifndef _SPRITE2_RENDER_COLOR_H_
#define _SPRITE2_RENDER_COLOR_H_

#include "Color.h"
#include "ObjectPool.h"

namespace s2
{

class RenderColor
{
public:
	RenderColor();
	RenderColor(const RenderColor& col);
	RenderColor& operator = (const RenderColor& col);

	bool operator == (const RenderColor& col) const;

	RenderColor operator * (const RenderColor& rc) const;

	Color GetMul() const { Color ret; ret.FromABGR(m_state.colors[IDX_MUL]); return ret; }
	Color GetAdd() const { Color ret; ret.FromABGR(m_state.colors[IDX_ADD]); return ret; }
	Color GetRMap() const { Color ret; ret.FromABGR(m_state.colors[IDX_RMAP]); return ret; }
	Color GetGMap() const { Color ret; ret.FromABGR(m_state.colors[IDX_GMAP]); return ret; }
	Color GetBMap() const { Color ret; ret.FromABGR(m_state.colors[IDX_BMAP]); return ret; }

	void SetMul(const Color& mul) { m_state.colors[IDX_MUL] = mul.ToABGR(); }
	void SetAdd(const Color& add) { m_state.colors[IDX_ADD] = add.ToABGR(); }
	void SetRMap(const Color& rmap) { m_state.colors[IDX_RMAP] = rmap.ToABGR(); }
	void SetGMap(const Color& gmap) { m_state.colors[IDX_GMAP] = gmap.ToABGR(); }
	void SetBMap(const Color& bmap) { m_state.colors[IDX_BMAP] = bmap.ToABGR(); }

	uint32_t GetMulABGR() const { return m_state.colors[IDX_MUL]; }
	uint32_t GetAddABGR() const { return m_state.colors[IDX_ADD]; }
	uint32_t GetRMapABGR() const { return m_state.colors[IDX_RMAP]; }
	uint32_t GetGMapABGR() const { return m_state.colors[IDX_GMAP]; }
	uint32_t GetBMapABGR() const { return m_state.colors[IDX_BMAP]; }

	void SetMulABGR(uint32_t mul) { m_state.colors[IDX_MUL] = mul; }
	void SetAddABGR(uint32_t add) { m_state.colors[IDX_ADD] = add; }
	void SetRMapABGR(uint32_t rmap) { m_state.colors[IDX_RMAP] = rmap; }
	void SetGMapABGR(uint32_t gmap) { m_state.colors[IDX_GMAP] = gmap; }
	void SetBMapABGR(uint32_t bmap) { m_state.colors[IDX_BMAP] = bmap; }

	/**
	 *  @interface
	 *    ObjectPool
	 */
	void Init();
	void Term() {}
	RenderColor* GetNext() const { return m_state.next; }
	void SetNext(RenderColor* next) { m_state.next = next; }

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

	union 
	{
		uint32_t colors[COLOR_MAX]; // in ABGR
		RenderColor* next;
	} m_state;

}; // RenderColor

typedef ObjectPool<RenderColor> RenderColorPool;

}

#endif // _SPRITE2_RENDER_COLOR_H_