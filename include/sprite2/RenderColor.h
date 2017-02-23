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

	RenderColor operator * (const RenderColor& rc) const;

	const Color& GetMul() const { return m_state.mul; }
	const Color& GetAdd() const { return m_state.add; }
	const Color& GetMapR() const { return m_state.rmap; }
	const Color& GetMapG() const { return m_state.gmap; }
	const Color& GetMapB() const { return m_state.bmap; }

	void SetMul(const Color& mul) { m_state.mul = mul; }
	void SetAdd(const Color& add) { m_state.add = add; }
	void SetMapR(const Color& rmap) { m_state.rmap = rmap; }
	void SetMapG(const Color& gmap) { m_state.gmap = gmap; }
	void SetMapB(const Color& bmap) { m_state.bmap = bmap; }

	/**
	 *  @interface
	 *    ObjectPool
	 */
	void Init();
	void Term() {}
	RenderColor* GetNext() const { return m_state.next; }
	void SetNext(RenderColor* next) { m_state.next = next; }

private:
	union 
	{
		struct 
		{
			Color mul, add;
			Color rmap, gmap, bmap;
		};

		RenderColor* next;

	} m_state;

}; // RenderColor

typedef ObjectPool<RenderColor> RenderColorPool;

}

#endif // _SPRITE2_RENDER_COLOR_H_