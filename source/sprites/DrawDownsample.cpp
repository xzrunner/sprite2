#include "DrawDownsample.h"
#include "S2_RenderTarget.h"
#include "S2_RenderTargetMgr.h"
#include "RenderScissor.h"
#include "RenderCtxStack.h"
#include "S2_Sprite.h"
#include "S2_Symbol.h"
#include "RenderParams.h"
#include "SymType.h"

#include <unirender/UR_RenderContext.h>
#include <shaderlab/ShaderMgr.h>
#include <shaderlab/Sprite2Shader.h>

namespace s2
{

void DrawDownsample::Draw(const Sprite* spr, const RenderParams& rp, float downsample)
{
	if (downsample <= 0) {
		return;
	}

	RenderTargetMgr* RT = RenderTargetMgr::Instance();
	RenderTarget* rt = RT->Fetch();
	if (!rt) {
		return;
	}

	sl::ShaderMgr::Instance()->FlushShader();

	RenderScissor::Instance()->Close();
	RenderCtxStack::Instance()->Push(RenderContext(RT->WIDTH, RT->HEIGHT, RT->WIDTH, RT->HEIGHT));

	rt->Bind();
	DrawSpr2RT(spr, rp, downsample);
	rt->Unbind();

	RenderCtxStack::Instance()->Pop();
	RenderScissor::Instance()->Open();

	DrawRT2Screen(rt->GetTexID(), spr, rp, downsample);

	RT->Return(rt);
}

void DrawDownsample::DrawSpr2RT(const Sprite* spr, const RenderParams& rp, float downsample)
{
	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
	mgr->GetContext()->Clear(0);

	RenderParams _rp(rp);
	_rp.mt.Identity();
	_rp.mt.Scale(downsample, downsample);

	spr->SetMatDisable(true);
	spr->GetSymbol()->Draw(_rp, spr);
	spr->SetMatDisable(false);

	mgr->FlushShader();
}

void DrawDownsample::DrawRT2Screen(int tex_id, const Sprite* spr, const RenderParams& rp, float downsample)
{
	RenderTargetMgr* RT = RenderTargetMgr::Instance();

	S2_MAT t = spr->GetLocalMat() * rp.mt;

	sm::rect r;
	if (spr->GetSymbol()->Type() == SYM_PARTICLE3D) {
// 		float hw = RT->WIDTH * downsample;
// 		float hh = RT->HEIGHT * downsample;
		float hw = RT->WIDTH * 0.5f;
		float hh = RT->HEIGHT * 0.5f;
		r.xmin = -hw;
		r.xmax =  hw;
		r.ymin = -hh;
		r.ymax =  hh;
	} else {
		r = spr->GetSymbol()->GetBounding();
	}

	sm::vec2 vertices[4];
	vertices[0] = sm::vec2(r.xmin, r.ymin);
	vertices[1] = sm::vec2(r.xmin, r.ymax);
	vertices[2] = sm::vec2(r.xmax, r.ymax);
	vertices[3] = sm::vec2(r.xmax, r.ymin);
	for (int i = 0; i < 4; ++i) {
		vertices[i] = t * vertices[i];
	}

	sm::vec2 texcoords[4];
	texcoords[0] = sm::vec2(r.xmin, r.ymin);
	texcoords[1] = sm::vec2(r.xmin, r.ymax);
	texcoords[2] = sm::vec2(r.xmax, r.ymax);
	texcoords[3] = sm::vec2(r.xmax, r.ymin);
	S2_MAT mt;
	mt.Scale(downsample, downsample);
	for (int i = 0; i < 4; ++i) {
		texcoords[i] = mt * texcoords[i];
	}
	for (int i = 0; i < 4; ++i) {
		texcoords[i].x = texcoords[i].x / RT->WIDTH  + 0.5f;
		texcoords[i].y = texcoords[i].y / RT->HEIGHT + 0.5f;
	}

	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
	mgr->SetShader(sl::SPRITE2);
	sl::Sprite2Shader* shader = static_cast<sl::Sprite2Shader*>(mgr->GetShader(sl::SPRITE2));
	shader->SetColor(0xffffffff, 0);
	shader->SetColorMap(0x000000ff, 0x0000ff00, 0x00ff0000);
	shader->DrawQuad(&vertices[0].x, &texcoords[0].x, tex_id);
}

}