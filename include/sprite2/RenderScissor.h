#ifndef _SPRITE2_RENDER_SCISSOR_H_
#define _SPRITE2_RENDER_SCISSOR_H_

#include <CU_Singleton.h>
#include <SM_Rect.h>

#include <vector>

namespace s2
{

class RenderScissor
{
public:
	void Push(float x, float y, float w, float h, bool use_render_screen = true);
	void Pop(bool use_render_screen = true);

	bool Empty() const { return m_stack.empty(); }

	void Close();
	void Open();

	bool IsOutside(const sm::rect& r) const;

public:
	struct Rect
	{
		float x, y, w, h;
	};

private:
	static void Intersection(const Rect& r, float& x, float& y, float& w, float& h);

private:
	std::vector<Rect> m_stack;
	
	SINGLETON_DECLARATION(RenderScissor)

}; // RenderScissor

}

#endif // _SPRITE2_RENDER_SCISSOR_H_