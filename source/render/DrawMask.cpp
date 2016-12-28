#include "DrawMask.h"
#include "RenderCtxStack.h"
#include "RenderParams.h"
#include "DrawNode.h"
#include "S2_Sprite.h"
#include "S2_Symbol.h"
#include "RenderScissor.h"
#include "RenderTarget.h"

#include <SM_Rect.h>
#include <unirender/RenderTarget.h>
#include <unirender/Texture.h>
#include <unirender/RenderContext.h>
#include <shaderlab/ShaderMgr.h>
#include <shaderlab/Shader.h>
#include <shaderlab/MaskShader.h>

namespace s2
{

void DrawMask::Draw(const Sprite* base, const Sprite* mask, const RenderParams& params)
{
	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
	mgr->FlushShader();

	RenderScissor::Instance()->Close();

	const ur::RenderTarget* rt0 = RenderTarget::Instance()->GetRT0();
	int w = rt0->GetTexture()->Width(),
		h = rt0->GetTexture()->Height();
	RenderCtxStack::Instance()->Push(RenderCtx(w, h, w, h));

	DrawBaseToFbo0(base, params.color);
	DrawMaskToFbo1(mask);

	RenderCtxStack::Instance()->Pop();

	RenderScissor::Instance()->Open();

	DrawMashFromFbo(mask, params.mt);
}

void DrawMask::DrawBaseToFbo0(const Sprite* base, const RenderColor& rc)
{
	RenderTarget::Instance()->GetRT0()->Bind();

	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
	mgr->GetContext()->Clear(0);

	mgr->SetShader(sl::SPRITE2);
	sl::Shader* shader = mgr->GetShader();

	RenderParams params;
	params.set_shader = false;
	params.color = rc;
	DrawNode::Draw(base, params);

	shader->Commit();

	RenderTarget::Instance()->GetRT0()->Unbind();
}

void DrawMask::DrawMaskToFbo1(const Sprite* mask)
{
	RenderTarget::Instance()->GetRT1()->Bind();

	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
	mgr->GetContext()->Clear(0);

	mgr->SetShader(sl::SPRITE2);
	sl::Shader* shader = mgr->GetShader();

	RenderParams params;
	params.set_shader = false;
	DrawNode::Draw(mask, params);

	shader->Commit();

	RenderTarget::Instance()->GetRT1()->Unbind();
}

void DrawMask::DrawMashFromFbo(const Sprite* mask, const sm::mat4& mt)
{
	sm::vec2 vertices[4];
	sm::rect r = mask->GetSymbol()->GetBounding();
	vertices[0] = sm::vec2(r.xmin, r.ymin);
	vertices[1] = sm::vec2(r.xmin, r.ymax);
	vertices[2] = sm::vec2(r.xmax, r.ymax);
	vertices[3] = sm::vec2(r.xmax, r.ymin);

	sm::vec2 texcoords[4];
	const ur::RenderTarget* rt0 = RenderTarget::Instance()->GetRT0();
	int w0 = rt0->GetTexture()->Width(),
		h0 = rt0->GetTexture()->Height();
	for (int i = 0; i < 4; ++i) {
		texcoords[i] = vertices[i];
		texcoords[i].x = texcoords[i].x / w0 + 0.5f;
		texcoords[i].y = texcoords[i].y / h0 + 0.5f;
	}

	sm::vec2 texcoords_mask[4];
	const ur::RenderTarget* rt1 = RenderTarget::Instance()->GetRT1();
	int w1 = rt1->GetTexture()->Width(),
		h1 = rt1->GetTexture()->Height();
	for (int i = 0; i < 4; ++i) {
		texcoords_mask[i] = vertices[i];
		texcoords_mask[i].x = texcoords_mask[i].x / w1 + 0.5f;
		texcoords_mask[i].y = texcoords_mask[i].y / h1 + 0.5f;
	}

	for (int i = 0; i < 4; ++i) {
		vertices[i] = mt * vertices[i];
	}

	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
	mgr->SetShader(sl::MASK);
	sl::MaskShader* shader = static_cast<sl::MaskShader*>(mgr->GetShader());
	int tex0_id = RenderTarget::Instance()->GetRT0()->GetTexture()->ID(),
		tex1_id = RenderTarget::Instance()->GetRT1()->GetTexture()->ID();
	shader->Draw(&vertices[0].x, &texcoords[0].x, &texcoords_mask[0].x, tex0_id, tex1_id);
}

}