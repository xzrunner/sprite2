#include "DrawBlend.h"
#include "RenderCtxStack.h"
#include "S2_Sprite.h"
#include "S2_Symbol.h"
#include "RenderParams.h"
#include "DrawNode.h"
#include "RenderScissor.h"
#include "S2_RenderTargetMgr.h"
#include "S2_RenderTarget.h"
#ifndef S2_DISABLE_STATISTICS
#include "sprite2/StatPingPong.h"
#endif // S2_DISABLE_STATISTICS
#include "BoundingBox.h"

#include <SM_Rect.h>
#include <unirender/UR_RenderTarget.h>
#include <unirender/UR_Texture.h>
#include <unirender/UR_RenderContext.h>
#include <shaderlab/ShaderMgr.h>
#include <shaderlab/BlendShader.h>
#include <shaderlab/Sprite2Shader.h>

#include <assert.h>

namespace s2
{

RenderReturn DrawBlend::Draw(const Sprite* spr, const RenderParams& rp)
{
	RenderTargetMgr* RT = RenderTargetMgr::Instance();

	sm::rect sz;
	spr->GetBounding(rp.actor)->CombineTo(sz);
	const bool too_large = sz.Width() > RT->WIDTH || sz.Height() > RT->HEIGHT;

	RenderTarget* rt = too_large ? RT->FetchScreen() : RT->Fetch();
	if (!rt) {
		return RENDER_NO_RT;
	}

	RenderReturn ret = RENDER_OK;

#ifndef S2_DISABLE_STATISTICS
	StatPingPong::Instance()->AddBlend();
#endif // S2_DISABLE_STATISTICS

	assert(spr->GetShader().GetBlend() != BM_NULL);

	sl::ShaderMgr::Instance()->FlushShader();

	RenderScissor::Instance()->Disable();
	if (!too_large) {
		RenderCtxStack::Instance()->Push(RenderContext(RT->WIDTH, RT->HEIGHT, RT->WIDTH, RT->HEIGHT));
	}

	rt->Bind();
	ret |= DrawSpr2RT(spr, rp, too_large);
	rt->Unbind();

	if (!too_large) {
		RenderCtxStack::Instance()->Pop();
	}
	RenderScissor::Instance()->Enable();

	if (too_large) {
		ret |= DrawRT2ScreenLarge(rt->GetTexID());
		RT->ReturnScreen(rt);
	} else {
		ret |= DrawRT2Screen(rt->GetTexID(), spr, rp);
		RT->Return(rt);
	}

	return ret;
}

RenderReturn DrawBlend::DrawSpr2RT(const Sprite* spr, const RenderParams& rp, bool too_large)
{
	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
	sl::BlendShader* shader = static_cast<sl::BlendShader*>(mgr->GetShader(sl::BLEND));

	mgr->GetContext()->Clear(0);

	mgr->SetShader(sl::BLEND);
	BlendMode mode = spr->GetShader().GetBlend();
	shader->SetMode(mode);

	RenderParams* rp_child = RenderParamsPool::Instance()->Pop();
	*rp_child = rp;

	rp_child->SetChangeShader(false);
	rp_child->SetDisableBlend(true);
	if (too_large) {
		rp_child->vertex_offset.Set(0, 0);
	} else {
		rp_child->vertex_offset = - (rp_child->mt * spr->GetPosition());
	}
	RenderReturn ret = DrawNode::Draw(spr, *rp_child);

	RenderParamsPool::Instance()->Push(rp_child); 

	shader->Commit();

	return ret;
}

RenderReturn DrawBlend::DrawRT2Screen(int tex_id, const Sprite* spr, const RenderParams& rp)
{
	RenderTargetMgr* RT = RenderTargetMgr::Instance();

	S2_MAT t = spr->GetLocalMat() * rp.mt;
	sm::rect r = spr->GetSymbol()->GetBounding();

	sm::vec2 vertices[4];
	vertices[0] = sm::vec2(r.xmin, r.ymin);
	vertices[1] = sm::vec2(r.xmin, r.ymax);
	vertices[2] = sm::vec2(r.xmax, r.ymax);
	vertices[3] = sm::vec2(r.xmax, r.ymin);
	for (int i = 0; i < 4; ++i) {
		vertices[i] = t * vertices[i];
	}

	sm::vec2 vertex_offset = - (rp.mt * spr->GetPosition());

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
	shader->DrawQuad(&vertices[0].x, &texcoords[0].x, tex_id);

	return RENDER_OK;
}

RenderReturn DrawBlend::DrawRT2ScreenLarge(int tex_id)
{
	s2::RenderCtxStack::Instance()->Push(s2::RenderContext(2, 2, 0, 0));

	sl::ShaderMgr* sl_mgr = sl::ShaderMgr::Instance();
	sl_mgr->SetShader(sl::SPRITE2);
	sl::Sprite2Shader* sl_shader = static_cast<sl::Sprite2Shader*>(sl_mgr->GetShader());
	sl_shader->SetColor(0xffffffff, 0);
	sl_shader->SetColorMap(0x000000ff, 0x0000ff00, 0x00ff0000);

	float xmin = -1, ymin = -1;
	float xmax =  1, ymax =  1;
	float vertices[8], texcoords[8];
	vertices[0] = xmin; vertices[1] = ymin;
	vertices[2] = xmax; vertices[3] = ymin;
	vertices[4] = xmax; vertices[5] = ymax;
	vertices[6] = xmin; vertices[7] = ymax;
	texcoords[0] = 0; texcoords[1] = 0;
	texcoords[2] = 1; texcoords[3] = 0;
	texcoords[4] = 1; texcoords[5] = 1;
	texcoords[6] = 0; texcoords[7] = 1;

	sl::Sprite2Shader* shader = static_cast<sl::Sprite2Shader*>(sl_mgr->GetShader());
	shader->DrawQuad(vertices, texcoords, tex_id);

	s2::RenderCtxStack::Instance()->Pop();

	return RENDER_OK;
}

}