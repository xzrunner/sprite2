#include "DrawGaussianBlur.h"
#include "S2_Sprite.h"
#include "RenderCtxStack.h"
#include "BoundingBox.h"
#include "RenderParams.h"
#include "FilterFactory.h"
#include "DrawNode.h"
#include "RenderScissor.h"
#include "RenderTarget.h"

#include <unirender/RenderTarget.h>
#include <unirender/Texture.h>
#include <unirender/RenderContext.h>
#include <shaderlab/ShaderMgr.h>
#include <shaderlab/Sprite2Shader.h>
#include <shaderlab/FilterShader.h>
#include <shaderlab/GaussianBlurHoriProg.h>
#include <shaderlab/GaussianBlurVertProg.h>

namespace s2
{

void DrawGaussianBlur::Draw(const Sprite* spr, const RenderParams& params, int iterations)
{
	DrawToFbo0(spr, params, iterations);
	DrawToScreen(true, spr->GetPosition());
}

void DrawGaussianBlur::DrawToFbo0(const Sprite* spr, const RenderParams& params, int iterations)
{
	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
	mgr->GetContext()->Clear(0);

	RenderScissor::Instance()->Close();

	DrawInit(spr, params, true);

	const ur::RenderTarget* rt0 = RenderTarget::Instance()->GetRT0();
	int w = rt0->GetTexture()->Width(),
		h = rt0->GetTexture()->Height();
	RenderCtxStack::Instance()->Push(RenderCtx(w, h, w, h));

	DrawInit(spr, params, true);

	mgr->SetShader(sl::FILTER);

	sm::vec2 sz = spr->GetBounding()->GetSize().Size();
	for (int i = 0; i < iterations; ++i) {
		DrawBetweenFBO(true, true, params.color, sz.x);
		DrawBetweenFBO(false, false, params.color, sz.y);
	}

	RenderCtxStack::Instance()->Pop();

	RenderScissor::Instance()->Open();
}

void DrawGaussianBlur::DrawToScreen(bool is_target0, const sm::vec2& offset)
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

	int tex_id;
	if (is_target0) {
		tex_id = RenderTarget::Instance()->GetRT0()->GetTexture()->ID();
	} else {
		tex_id = RenderTarget::Instance()->GetRT1()->GetTexture()->ID();
	}
	shader->Draw(&vertices[0].x, &texcoords[0].x, tex_id);

	shader->Commit();
}

void DrawGaussianBlur::DrawInit(const Sprite* spr, const RenderParams& params, bool is_target0)
{
	if (is_target0) {
		RenderTarget::Instance()->GetRT0()->Bind();
	} else {
		RenderTarget::Instance()->GetRT1()->Bind();
	}
	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
	mgr->GetContext()->Clear(0);

	RenderParams _params = params;
	const sm::vec2& offset = spr->GetPosition();
	_params.mt.Translate(-offset.x, -offset.y, 0);
	_params.set_shader = false;
	_params.shader.SetFilter(FM_NULL);
	_params.disable_filter = true;

	mgr->SetShader(sl::SPRITE2);
	DrawNode::Draw(spr, _params);

	if (is_target0) {
		RenderTarget::Instance()->GetRT0()->Unbind();
	} else {
		RenderTarget::Instance()->GetRT1()->Unbind();
	}
}

void DrawGaussianBlur::DrawBetweenFBO(bool is_t0_to_t1, bool hori, const RenderColor& col, float tex_size)
{
	ur::RenderTarget* rt = NULL;
	if (is_t0_to_t1) {
		rt = RenderTarget::Instance()->GetRT0();
	} else {
		rt = RenderTarget::Instance()->GetRT1();
	}
	
	int src_tex_w = rt->GetTexture()->Width(),
		src_tex_h = rt->GetTexture()->Height();
	int src_tex_id = rt->GetTexture()->ID();
	rt->Bind();
	
	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
	mgr->GetContext()->Clear(0);
	
	sl::FilterShader* shader = static_cast<sl::FilterShader*>(mgr->GetShader(sl::FILTER));
	if (hori) {
		shader->SetMode(sl::FM_GAUSSIAN_BLUR_HORI);
		sl::GaussianBlurHoriProg* prog = static_cast<sl::GaussianBlurHoriProg*>(shader->GetProgram(sl::FM_GAUSSIAN_BLUR_HORI));
		prog->SetTexWidth(src_tex_w);
//		prog->SetTexWidth(tex_size);
	} else {
		shader->SetMode(sl::FM_GAUSSIAN_BLUR_VERT);
		sl::GaussianBlurVertProg* prog = static_cast<sl::GaussianBlurVertProg*>(shader->GetProgram(sl::FM_GAUSSIAN_BLUR_VERT));
		prog->SetTexHeight(src_tex_w);
//		prog->SetTexHeight(tex_size);		
	}
	shader->SetColor(col.mul.ToABGR(), col.add.ToABGR());

	sm::vec2 vertices[4], texcoords[4];
	vertices[0].Set(-512, -512);
	vertices[1].Set( 512, -512);
	vertices[2].Set( 512,  512);
	vertices[3].Set(-512,  512);
	texcoords[0].Set(0, 0);
	texcoords[1].Set(1, 0);
	texcoords[2].Set(1, 1);
	texcoords[3].Set(0, 1);

	shader->Draw(&vertices[0].x, &texcoords[0].x, src_tex_id);

	shader->Commit();

	if (is_t0_to_t1) {
		RenderTarget::Instance()->GetRT1()->Unbind();
	} else {
		RenderTarget::Instance()->GetRT0()->Unbind();
	}
}

}