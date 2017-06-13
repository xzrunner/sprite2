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
	RenderTarget* rt = RT->Fetch();
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
	RenderCtxStack::Instance()->Push(RenderContext(RT->WIDTH, RT->HEIGHT, RT->WIDTH, RT->HEIGHT));

	rt->Bind();
	ret |= DrawSpr2RT(spr, rp);
	rt->Unbind();

	RenderCtxStack::Instance()->Pop();
	RenderScissor::Instance()->Enable();

	ret |= DrawRT2Screen(rt->GetTexID(), spr, rp);

	RT->Return(rt);

	return ret;
}

RenderReturn DrawBlend::DrawSpr2RT(const Sprite* spr, const RenderParams& rp)
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
	rp_child->vertex_offset = - (rp_child->mt * spr->GetPosition());
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

}