#include "DrawGaussianBlur.h"
#include "S2_Sprite.h"
#include "RenderCtxStack.h"
#include "BoundingBox.h"
#include "RenderParams.h"
#include "FilterFactory.h"
#include "DrawNode.h"
#include "RenderScissor.h"
#include "S2_RenderTargetMgr.h"
#include "S2_RenderTarget.h"
#ifndef S2_DISABLE_STATISTICS
#include "sprite2/Statistics.h"
#endif // S2_DISABLE_STATISTICS

#include <unirender/UR_RenderContext.h>
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

	Statistics::Instance()->AddGaussianBlur();

	RenderTargetMgr* RT = RenderTargetMgr::Instance();
	RenderTarget* tmp_rt = RT->Fetch();

	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
	mgr->FlushShader();

	RenderScissor::Instance()->Disable();
	RenderCtxStack::Instance()->Push(RenderContext(RT->WIDTH, RT->HEIGHT, RT->WIDTH, RT->HEIGHT));

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

	RenderParams* rp_child = RenderParamsPool::Instance()->Pop();
	*rp_child = rp;

	const sm::vec2& offset = spr->GetPosition();
	rp_child->mt.Translate(-offset.x, -offset.y);
	rp_child->SetChangeShader(false);
	rp_child->shader.SetFilter(FM_NULL);
	rp_child->SetDisableFilter(true);

	mgr->SetShader(sl::SPRITE2);
	RenderReturn ret = DrawNode::Draw(spr, *rp_child);

	rt->Unbind();

	RenderParamsPool::Instance()->Push(rp_child); 

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
		prog->SetTexWidth(RT->WIDTH);
	} else {
		shader->SetMode(sl::FM_GAUSSIAN_BLUR_VERT);
		sl::GaussianBlurVertProg* prog = static_cast<sl::GaussianBlurVertProg*>(shader->GetProgram(sl::FM_GAUSSIAN_BLUR_VERT));
		prog->SetTexHeight(RT->HEIGHT);
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
	Statistics::Instance()->AddOverdrawArea(1);
#endif // S2_DISABLE_STATISTICS

	shader->Draw(&vertices[0].x, &texcoords[0].x, src->GetTexID());

	shader->Commit();

	dst->Unbind();

	return RENDER_OK;
}

}