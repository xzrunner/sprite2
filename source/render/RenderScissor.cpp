#include "RenderScissor.h"
#include "RenderScreen.h"

#include <unirender/RenderContext.h>
#include <shaderlab/ShaderMgr.h>

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

void RenderScissor::Push(float x, float y, float w, float h)
{
	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
	mgr->FlushShader();
	if (m_stack.empty()) {
		mgr->GetContext()->EnableScissor(true);
	}

	Rect r;
	r.x = x;
	r.y = y;
	r.w = w;
	r.h = h;
	m_stack.push_back(r);

	RenderScreen::Scissor(x, y, w, h);
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
	}

	const Rect& r = m_stack.back();
	RenderScreen::Scissor(r.x, r.y, r.w, r.h);
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

}