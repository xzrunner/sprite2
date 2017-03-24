#include "RenderScissor.h"
#include "RenderScreen.h"

#include <unirender/UR_RenderContext.h>
#include <shaderlab/ShaderMgr.h>
#include <SM_Test.h>

#include <assert.h>

namespace s2
{

SINGLETON_DEFINITION(RenderScissor)

RenderScissor::RenderScissor() 
{
}

RenderScissor::~RenderScissor() 
{
}

void RenderScissor::Push(float x, float y, float w, float h, bool use_render_screen, bool no_intersect)
{
	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
	mgr->FlushShader();
	if (m_stack.empty()) {
		mgr->GetContext()->EnableScissor(true);
	}

	if (!no_intersect && !m_stack.empty()) {
		Intersection(m_stack.back(), x, y, w, h);
	}

	Rect r;
	r.x = x;
	r.y = y;
	r.w = w;
	r.h = h;
	m_stack.push_back(r);

	if (use_render_screen) {
		RenderScreen::Scissor(x, y, w, h);
	} else {
		sl::ShaderMgr::Instance()->GetContext()->SetScissor(x, y, w, h);
	}
}

void RenderScissor::Pop(bool use_render_screen)
{
	assert(!m_stack.empty());

	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
	mgr->FlushShader();
	m_stack.pop_back();
	if (m_stack.empty()) {
		mgr->GetContext()->EnableScissor(false);
		return;
	}

	const Rect& r = m_stack.back();
	if (use_render_screen) {
		RenderScreen::Scissor(r.x, r.y, r.w, r.h);
	} else {
		sl::ShaderMgr::Instance()->GetContext()->SetScissor(r.x, r.y, r.w, r.h);
	}
}

void RenderScissor::Close()
{
	if (m_stack.empty()) {
		return;
	}

	sl::ShaderMgr::Instance()->GetContext()->EnableScissor(false);
}

void RenderScissor::Open()
{
	if (m_stack.empty()) {
		return;
	}

	sl::ShaderMgr::Instance()->GetContext()->EnableScissor(true);
	const Rect& r = m_stack.back();
	RenderScreen::Scissor(r.x, r.y, r.w, r.h);
}

bool RenderScissor::IsOutside(const sm::rect& r) const
{
	for (int i = 0, n = m_stack.size(); i < n; ++i) {
		const Rect& rr = m_stack[i];
		if (!sm::is_rect_intersect_rect(sm::rect(rr.x, rr.y, rr.x + rr.w, rr.y + rr.h), r)) {
			return true;
		}
	}
	return false;
}

void RenderScissor::Intersection(const Rect& r, float& x, float& y, float& w, float& h)
{
	float newx = r.x > x ? r.x : x,
		  newy = r.y > y ? r.y : y;

	float bx = r.x + r.w,
		  by = r.y + r.h;
	float ax = x + w,
		  ay = y + h;
	float neww = (bx > ax ? ax : bx) - newx,
		  newh = (by > ay ? ay : by) - newy;

	x = newx;
	y = newy;
	w = neww;
	h = newh;
}

}