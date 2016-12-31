#include "DrawMask.h"
#include "RenderCtxStack.h"
#include "RenderParams.h"
#include "DrawNode.h"
#include "S2_Sprite.h"
#include "S2_Symbol.h"
#include "RenderScissor.h"
#include "RenderTarget.h"

#include <SM_Rect.h>
#include <unirender/RenderContext.h>
#include <shaderlab/ShaderMgr.h>
#include <shaderlab/Shader.h>
#include <shaderlab/MaskShader.h>

namespace s2
{

void DrawMask::Draw(const Sprite* base, const Sprite* mask, const RenderParams& params)
{
	RenderTarget* RT = RenderTarget::Instance();

	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
	mgr->FlushShader();

	RenderScissor::Instance()->Close();
	RenderCtxStack::Instance()->Push(RenderCtx(RT->WIDTH, RT->HEIGHT, RT->WIDTH, RT->HEIGHT));

	int rt_base = RT->Fetch();
	if (rt_base == -1) {
		RenderCtxStack::Instance()->Pop();
		RenderScissor::Instance()->Open();
		return;
	}
	DrawBaseToRT(rt_base, base, params.color);

	int rt_mask = RT->Fetch();
	if (rt_mask == -1) {
		RT->Return(rt_base);
		RenderCtxStack::Instance()->Pop();
		RenderScissor::Instance()->Open();
		return;
	}
	DrawMaskToRT(rt_mask, mask);

	RenderCtxStack::Instance()->Pop();
	RenderScissor::Instance()->Open();

	DrawMaskFromRT(rt_base, rt_mask, mask, params.mt);

	RT->Return(rt_base);
	RT->Return(rt_mask);
}

void DrawMask::DrawBaseToRT(int rt, const Sprite* base, const RenderColor& rc)
{
	RenderTarget::Instance()->Bind(rt);

	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
	mgr->GetContext()->Clear(0);

	mgr->SetShader(sl::SPRITE2);
	sl::Shader* shader = mgr->GetShader();

	RenderParams params;
	params.set_shader = false;
	params.color = rc;
	DrawNode::Draw(base, params);

	shader->Commit();

	RenderTarget::Instance()->Unbind(rt);
}

void DrawMask::DrawMaskToRT(int rt, const Sprite* mask)
{
	RenderTarget::Instance()->Bind(rt);

	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
	mgr->GetContext()->Clear(0);

	mgr->SetShader(sl::SPRITE2);
	sl::Shader* shader = mgr->GetShader();

	RenderParams params;
	params.set_shader = false;
	DrawNode::Draw(mask, params);

	shader->Commit();

	RenderTarget::Instance()->Unbind(rt);
}

void DrawMask::DrawMaskFromRT(int rt_base, int rt_mask, const Sprite* mask, const sm::mat4& mt)
{
	RenderTarget* RT = RenderTarget::Instance();

	sm::vec2 vertices[4];
	sm::rect r = mask->GetSymbol()->GetBounding();
	vertices[0] = sm::vec2(r.xmin, r.ymin);
	vertices[1] = sm::vec2(r.xmin, r.ymax);
	vertices[2] = sm::vec2(r.xmax, r.ymax);
	vertices[3] = sm::vec2(r.xmax, r.ymin);

	sm::vec2 texcoords[4];
	for (int i = 0; i < 4; ++i) {
		texcoords[i] = vertices[i];
		texcoords[i].x = texcoords[i].x / RT->WIDTH  + 0.5f;
		texcoords[i].y = texcoords[i].y / RT->HEIGHT + 0.5f;
	}

	sm::vec2 texcoords_mask[4];
	for (int i = 0; i < 4; ++i) {
		texcoords_mask[i] = vertices[i];
		texcoords_mask[i].x = texcoords_mask[i].x / RT->WIDTH  + 0.5f;
		texcoords_mask[i].y = texcoords_mask[i].y / RT->HEIGHT + 0.5f;
	}

	for (int i = 0; i < 4; ++i) {
		vertices[i] = mt * vertices[i];
	}

	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
	mgr->SetShader(sl::MASK);
	sl::MaskShader* shader = static_cast<sl::MaskShader*>(mgr->GetShader());
	int tex_base = RT->GetTexID(rt_base),
		tex_mask = RT->GetTexID(rt_mask);
	shader->Draw(&vertices[0].x, &texcoords[0].x, &texcoords_mask[0].x, tex_base, tex_mask);
}

}