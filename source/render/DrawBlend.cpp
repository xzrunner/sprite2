#include "DrawBlend.h"
#include "sprite2/StatPingPong.h"
#include "S2_Sprite.h"
#include "S2_Symbol.h"
#include "RenderParams.h"
#include "DrawNode.h"
#include "S2_RenderTargetMgr.h"

#include <unirender/UR_RenderContext.h>
#include <shaderlab/ShaderMgr.h>
#include <shaderlab/BlendShader.h>
#include <shaderlab/Sprite2Shader.h>

namespace s2
{

DrawBlend::DrawBlend()
	: DrawPingPong(StatPingPong::BLEND)
{
}

RenderReturn DrawBlend::DrawSpr2RT(const Sprite* spr, const RenderParams& rp, bool too_large) const
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

RenderReturn DrawBlend::DrawRT2ScreenSmall(int tex_id, const Sprite* spr, 
										   const RenderParams& rp, bool reset_color) const
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

	sm::vec2 texcoords[4];
	sm::vec2 vertex_offset = - (rp.mt * spr->GetPosition());
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