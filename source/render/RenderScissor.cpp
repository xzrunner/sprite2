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
	mgr->GetContext()->EnableScissor(true);

	if (!no_intersect && !m_stack.empty()) {
		Intersection(m_stack.back(), x, y, w, h);
	}

	Rect r;
	r.x = x;
	r.y = y;
	r.w = w;
	r.h = h;
	r.use_render_screen = use_render_screen;
	m_stack.push_back(r);

	if (use_render_screen) {
		RenderScreen::Scissor(x, y, w, h);
	} else {
		sl::ShaderMgr::Instance()->GetContext()->SetScissor(x, y, w, h);
	}
}

void RenderScissor::Pop()
{
	assert(!m_stack.empty());

	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
	mgr->FlushShader();
	m_stack.pop_back();
	if (m_stack.empty()) {
		mgr->GetContext()->EnableScissor(false);
		return;
	} else {
		mgr->GetContext()->EnableScissor(true);
	}

	const Rect& r = m_stack.back();
	if (r.IsInvalid()) {
		mgr->GetContext()->EnableScissor(false);
		return;
	}
	if (r.use_render_screen) {
		RenderScreen::Scissor(r.x, r.y, r.w, r.h);
	} else {
		sl::ShaderMgr::Instance()->GetContext()->SetScissor(r.x, r.y, r.w, r.h);
	}
}

bool RenderScissor::IsEmpty() const 
{ 
	return m_stack.empty() || m_stack.back().IsInvalid();
}

void RenderScissor::Disable()
{
	sl::ShaderMgr::Instance()->GetContext()->EnableScissor(false);
	Rect r;
	r.MakeInvalid();
	m_stack.push_back(r);
}

void RenderScissor::Enable()
{
	Pop();
}

bool RenderScissor::CullingTestOutside(const sm::rect& r) const
{
	if (m_stack.empty()) {
		return false;
	}
	const Rect& rr = m_stack.back();
	if (rr.IsInvalid()) {
		return true;
	} else {
		return !sm::is_rect_intersect_rect(sm::rect(rr.x, rr.y, rr.x + rr.w, rr.y + rr.h), r);
	}
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