#ifndef _SPRITE2_RENDER_SCISSOR_H_
#define _SPRITE2_RENDER_SCISSOR_H_

#include <cu/cu_macro.h>
#include <cu/cu_stl.h>
#include <SM_Rect.h>

namespace s2
{

class RenderScissor
{
public:
	void Push(float x, float y, float w, float h, bool use_render_screen, bool no_intersect);
	void Pop();

	bool IsEmpty() const;

	void Disable();
	void Enable();

	bool CullingTestOutside(const sm::rect& r) const;

public:
	class Rect
	{
	public:
		float x, y, w, h;
		bool use_render_screen;

	public:
		Rect() : x(0), y(0), w(0), h(0), use_render_screen(false) {}

		void MakeInvalid() {
			x = y = w = h = FLT_MAX;
		}
		bool IsInvalid() const {
			return x == FLT_MAX && y == FLT_MAX
				&& w == FLT_MAX && h == FLT_MAX;
		}
	}; // Rect

private:
	static void Intersection(const Rect& r, float& x, float& y, float& w, float& h);

private:
	CU_VEC<Rect> m_stack;
	
	CU_SINGLETON_DECLARATION(RenderScissor)

}; // RenderScissor

}

#endif // _SPRITE2_RENDER_SCISSOR_H_