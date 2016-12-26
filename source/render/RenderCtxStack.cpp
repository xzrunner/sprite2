#include "RenderCtxStack.h"

#include <unirender/RenderContext.h>
#include <shaderlab/SubjectMVP2.h>
#include <shaderlab/ShaderMgr.h>

namespace s2
{

SINGLETON_DEFINITION(RenderCtxStack)

RenderCtxStack::RenderCtxStack() 
{
}

RenderCtxStack::~RenderCtxStack() 
{
}

void RenderCtxStack::Push(const RenderCtx& ctx, bool set_vp)
{
	BindCtx(ctx, set_vp);
	m_stack.push_back(ctx);
}

void RenderCtxStack::Pop(bool set_vp)
{
	if (m_stack.empty()) {
		return;
	}

	m_stack.pop_back();

	if (!m_stack.empty()) {
		BindCtx(m_stack.back(), set_vp);		
	}
}

const RenderCtx* RenderCtxStack::Top() const
{
	if (m_stack.empty()) {
		return NULL;
	} else {
		return &m_stack[m_stack.size() - 1];
	}
}

void RenderCtxStack::BindCtx(const RenderCtx& ctx, bool set_vp)
{
	sl::SubjectMVP2* mvp2 = sl::SubjectMVP2::Instance();
	mvp2->NotifyModelview(ctx.mv_offset.x, ctx.mv_offset.y, ctx.mv_scale, ctx.mv_scale);
	mvp2->NotifyProjection(ctx.proj_width, ctx.proj_height);
	if (set_vp) {
		sl::ShaderMgr::Instance()->GetContext()->SetViewport(0, 0, ctx.screen_width, ctx.screen_height);
	}
}

}