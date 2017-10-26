#include "sprite2/DrawGaussianBlur.h"
#include "sprite2/Sprite.h"
#include "sprite2/RenderCtxStack.h"
#include "sprite2/BoundingBox.h"
#include "sprite2/RenderParams.h"
#include "sprite2/FilterFactory.h"
#include "sprite2/DrawNode.h"
#include "sprite2/RenderScissor.h"
#include "sprite2/RenderTargetMgr.h"
#include "sprite2/RenderTarget.h"
#ifndef S2_DISABLE_STATISTICS
#include "sprite2/StatPingPong.h"
#include "sprite2/StatOverdraw.h"
#endif // S2_DISABLE_STATISTICS

#include <memmgr/Allocator.h>
#include <unirender/RenderContext.h>
#include <shaderlab/ShaderMgr.h>
#include <shaderlab/Sprite2Shader.h>
#include <shaderlab/FilterShader.h>
#include <shaderlab/GaussianBlurHoriProg.h>
#include <shaderlab/GaussianBlurVertProg.h>

namespace s2
{

RenderReturn DrawGaussianBlur::Draw(const Sprite* spr, const RenderParams& rp, int iterations)
{
	RenderReturn ret = RENDER_OK;

	RenderTargetMgr* RT = RenderTargetMgr::Instance();
	RenderTarget* rt = RT->Fetch();

	ret |= DrawBlurToRT(rt, spr, rp, iterations);
	ret |= DrawFromRT(rt, spr->GetPosition());

	RT->Return(rt);

	return ret;
}

RenderReturn DrawGaussianBlur::DrawBlurToRT(RenderTarget* rt, const Sprite* spr, const RenderParams& rp, int iterations)
{	
	RenderReturn ret = RENDER_OK;

#ifndef S2_DISABLE_STATISTICS
	StatPingPong::Instance()->AddCount(StatPingPong::GAUSSIAN_BLUR);
#endif // S2_DISABLE_STATISTICS

	RenderTargetMgr* RT = RenderTargetMgr::Instance();
	RenderTarget* tmp_rt = RT->Fetch();

	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
	mgr->FlushShader();

	RenderScissor::Instance()->Disable();
	RenderCtxStack::Instance()->Push(RenderContext(
		static_cast<float>(RT->WIDTH), static_cast<float>(RT->HEIGHT), RT->WIDTH, RT->HEIGHT));

	ret |= DrawInit(rt, spr, rp);

	mgr->SetShader(sl::FILTER);

	sm::vec2 sz = spr->GetBounding()->GetSize().Size();
	for (int i = 0; i < iterations; ++i) 
	{
		ret |= DrawBetweenRT(rt, tmp_rt, true, rp.color, sz.x);
		ret |= DrawBetweenRT(tmp_rt, rt, false, rp.color, sz.y);
	}

	RenderCtxStack::Instance()->Pop();
	RenderScissor::Instance()->Enable();

	RT->Return(tmp_rt);

	return ret;
}

RenderReturn DrawGaussianBlur::DrawFromRT(RenderTarget* rt, const sm::vec2& offset)
{
	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
	mgr->SetShader(sl::SPRITE2);
	sl::Sprite2Shader* shader = static_cast<sl::Sprite2Shader*>(mgr->GetShader());

	sm::vec2 vertices[4];
	vertices[0].Set(-512, -512);
	vertices[1].Set( 512, -512);
	vertices[2].Set( 512,  512);
	vertices[3].Set(-512,  512);
	for (int i = 0; i < 4; ++i) {
		vertices[i] += offset;
	}

	sm::vec2 texcoords[4];
	texcoords[0].Set(0, 0);
	texcoords[1].Set(1, 0);
	texcoords[2].Set(1, 1);
	texcoords[3].Set(0, 1);

	shader->DrawQuad(&vertices[0].x, &texcoords[0].x, rt->GetTexID());

	shader->Commit();

	return RENDER_OK;
}

RenderReturn DrawGaussianBlur::DrawInit(RenderTarget* rt, const Sprite* spr, const RenderParams& rp)
{
	rt->Bind();

	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
	mgr->GetContext()->Clear(0);

	RenderParamsProxy rp_proxy;
	RenderParams* rp_child = rp_proxy.obj;
	memcpy(rp_child, &rp, sizeof(rp));

	const sm::vec2& offset = spr->GetPosition();
	rp_child->mt.Translate(-offset.x, -offset.y);
	rp_child->SetChangeShader(false);
#ifdef S2_FILTER_FULL
	rp_child->render_filter = nullptr;
#else
	rp_child->render_filter = FM_NULL;
#endif // S2_FILTER_FULL
	rp_child->SetDisableFilter(true);

	mgr->SetShader(sl::SPRITE2);
	RenderReturn ret = DrawNode::Draw(nullptr, spr, *rp_child);

	rt->Unbind();

	return ret;
}

RenderReturn DrawGaussianBlur::DrawBetweenRT(RenderTarget* src, RenderTarget* dst, bool hori, const RenderColor& col, float tex_size)
{
	RenderTargetMgr* RT = RenderTargetMgr::Instance();

	dst->Bind();
	
	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
	mgr->GetContext()->Clear(0);
	
	sl::FilterShader* shader = static_cast<sl::FilterShader*>(mgr->GetShader(sl::FILTER));
	if (hori) {
		shader->SetMode(sl::FM_GAUSSIAN_BLUR_HORI);
		sl::GaussianBlurHoriProg* prog = static_cast<sl::GaussianBlurHoriProg*>(shader->GetProgram(sl::FM_GAUSSIAN_BLUR_HORI));
		prog->SetTexWidth(static_cast<float>(RT->WIDTH));
	} else {
		shader->SetMode(sl::FM_GAUSSIAN_BLUR_VERT);
		sl::GaussianBlurVertProg* prog = static_cast<sl::GaussianBlurVertProg*>(shader->GetProgram(sl::FM_GAUSSIAN_BLUR_VERT));
		prog->SetTexHeight(static_cast<float>(RT->HEIGHT));
	}
	shader->SetColor(col.GetMulABGR(), col.GetAddABGR());

	sm::vec2 vertices[4], texcoords[4];
	vertices[0].Set(-512, -512);
	vertices[1].Set( 512, -512);
	vertices[2].Set( 512,  512);
	vertices[3].Set(-512,  512);
	texcoords[0].Set(0, 0);
	texcoords[1].Set(1, 0);
	texcoords[2].Set(1, 1);
	texcoords[3].Set(0, 1);

#ifndef S2_DISABLE_STATISTICS
	StatOverdraw::Instance()->AddArea(1);
#endif // S2_DISABLE_STATISTICS

	shader->Draw(&vertices[0].x, &texcoords[0].x, src->GetTexID());

	shader->Commit();

	dst->Unbind();

	return RENDER_OK;
}

}