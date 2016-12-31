#include "DrawBlend.h"
#include "RenderCtxStack.h"
#include "S2_Sprite.h"
#include "S2_Symbol.h"
#include "RenderParams.h"
#include "DrawNode.h"
#include "RenderScissor.h"
#include "RenderTarget.h"

#include <SM_Rect.h>
#include <unirender/RenderTarget.h>
#include <unirender/Texture.h>
#include <unirender/RenderContext.h>
#include <shaderlab/ShaderMgr.h>
#include <shaderlab/BlendShader.h>
#include <shaderlab/Sprite2Shader.h>

#include <assert.h>

namespace s2
{

void DrawBlend::Draw(const Sprite* spr, const sm::mat4& mt)
{
	RenderTarget* RT = RenderTarget::Instance();
	int rt = RT->Fetch();
	if (rt == -1) {
		return;
	}

	assert(spr->GetShader().GetBlend() != BM_NULL);

	sl::ShaderMgr::Instance()->FlushShader();

	RenderScissor::Instance()->Close();
	RenderCtxStack::Instance()->Push(RenderCtx(RT->WIDTH, RT->HEIGHT, RT->WIDTH, RT->HEIGHT));

	DrawSpr2RT(rt, spr, mt);

	RenderCtxStack::Instance()->Pop();
	RenderScissor::Instance()->Open();

	DrawRT2Screen(rt, spr, mt);

	RT->Return(rt);
}

void DrawBlend::DrawSpr2RT(int rt, const Sprite* spr, const sm::mat4& mt)
{
	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
	sl::BlendShader* shader = static_cast<sl::BlendShader*>(mgr->GetShader(sl::BLEND));

	RenderTarget::Instance()->Bind(rt);
	mgr->GetContext()->Clear(0);

	mgr->SetShader(sl::BLEND);
	BlendMode mode = spr->GetShader().GetBlend();
	shader->SetMode(mode);

	RenderParams params;
	params.mt = mt;
	params.set_shader = false;
	params.disable_blend = true;
	params.vertex_offset = - (mt * spr->GetPosition());
	DrawNode::Draw(spr, params);

	shader->Commit();

	RenderTarget::Instance()->Unbind(rt);
}

void DrawBlend::DrawRT2Screen(int rt, const Sprite* spr, const sm::mat4& mt)
{
	RenderTarget* RT = RenderTarget::Instance();

	sm::mat4 t = spr->GetLocalMat() * mt;
	sm::rect r = spr->GetSymbol()->GetBounding();

	sm::vec2 vertices[4];
	vertices[0] = sm::vec2(r.xmin, r.ymin);
	vertices[1] = sm::vec2(r.xmin, r.ymax);
	vertices[2] = sm::vec2(r.xmax, r.ymax);
	vertices[3] = sm::vec2(r.xmax, r.ymin);
	for (int i = 0; i < 4; ++i) {
		vertices[i] = t * vertices[i];
	}

	sm::vec2 vertex_offset = - (mt * spr->GetPosition());

	sm::vec2 texcoords[4];
	for (int i = 0; i < 4; ++i) {
		texcoords[i] = vertices[i] + vertex_offset;
		texcoords[i].x = texcoords[i].x / RT->WIDTH  + 0.5f;
		texcoords[i].y = texcoords[i].y / RT->HEIGHT + 0.5f;
	}

	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
	mgr->SetShader(sl::SPRITE2);
	sl::Sprite2Shader* shader = static_cast<sl::Sprite2Shader*>(mgr->GetShader(sl::SPRITE2));
	shader->SetColor(0xffffffff, 0);
	shader->SetColorMap(0x000000ff, 0x0000ff00, 0x00ff0000);
	shader->Draw(&vertices[0].x, &texcoords[0].x, RT->GetTexID(rt));
}

}