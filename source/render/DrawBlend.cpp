#include "sprite2/DrawBlend.h"
#include "sprite2/Sprite.h"
#include "sprite2/Symbol.h"
#include "sprite2/RenderParams.h"
#include "sprite2/DrawNode.h"

#include <memmgr/Allocator.h>
#include <shaderlab/Blackboard.h>
#include <shaderlab/ShaderMgr.h>
#include <shaderlab/BlendShader.h>
#include <shaderlab/Sprite2Shader.h>
#include <shaderlab/RenderContext.h>
#include <unirender/RenderContext.h>
#include <unirender/Blackboard.h>
#ifndef S2_DISABLE_DEFERRED
#include <cooking/Facade.h>
#endif // S2_DISABLE_DEFERRED
#ifndef S2_DISABLE_STATISTICS
#include <stat/StatPingPong.h>
#endif // S2_DISABLE_STATISTICS
#include <painting2/RenderShader.h>
#include <painting2/RenderTargetMgr.h>
#include <painting2/RenderContext.h>
#include <painting2/Blackboard.h>
#include <painting2/Context.h>

namespace s2
{

DrawBlend::DrawBlend()
	: DrawPingPong(st::StatPingPong::BLEND)
{
}

pt2::RenderReturn DrawBlend::DrawSpr2RT(cooking::DisplayList* dlist, const Sprite* spr, 
	                               const RenderParams& rp, bool too_large) const
{
	pt2::BlendMode mode = spr->GetShader().GetBlend();
#ifdef S2_DISABLE_DEFERRED
	auto& rc = sl::Blackboard::Instance()->GetRenderContext();
	sl::BlendShader* shader = static_cast<sl::BlendShader*>(rc.GetShaderMgr().GetShader(sl::BLEND));

	ur::Blackboard::Instance()->GetRenderContext().Clear(0);
	rc.GetShaderMgr().SetShader(sl::BLEND);
	shader->SetMode(mode);
#else
	cooking::render_clear(dlist, 0);
	cooking::change_shader(dlist, sl::BLEND);
	cooking::set_shader_blend_mode(dlist, mode);
#endif // S2_DISABLE_DEFERRED

	RenderParamsProxy rp_proxy;
	RenderParams* rp_child = rp_proxy.obj;
	memcpy(rp_child, &rp, sizeof(rp));

	rp_child->SetChangeShader(false);
	rp_child->SetDisableBlend(true);
	if (too_large) {
		rp_child->vertex_offset.Set(0, 0);
	} else {
		rp_child->vertex_offset = - (rp_child->mt * spr->GetPosition());
	}
	pt2::RenderReturn ret = DrawNode::Draw(nullptr, spr, *rp_child);

#ifdef S2_DISABLE_DEFERRED
	shader->Commit();
#else
	cooking::flush_shader(dlist);
#endif // S2_DISABLE_DEFERRED

	return ret;
}

pt2::RenderReturn DrawBlend::DrawRT2ScreenSmall(cooking::DisplayList* dlist, int tex_id, const Sprite* spr,
										   const RenderParams& rp, bool reset_color) const
{
	auto& pt2_ctx = pt2::Blackboard::Instance()->GetContext();
	auto& rt_mgr = pt2_ctx.GetRTMgr();

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
		texcoords[i].x = texcoords[i].x / rt_mgr.WIDTH  + 0.5f;
		texcoords[i].y = texcoords[i].y / rt_mgr.HEIGHT + 0.5f;
	}

#ifdef S2_DISABLE_DEFERRED
	auto& shader_mgr = sl::Blackboard::Instance()->GetRenderContext().GetShaderMgr();

	shader_mgr.SetShader(sl::SPRITE2);

	sl::Sprite2Shader* shader = static_cast<sl::Sprite2Shader*>(shader_mgr.GetShader(sl::SPRITE2));
	shader->SetColor(0xffffffff, 0);
	shader->SetColorMap(0x000000ff, 0x0000ff00, 0x00ff0000);
	shader->DrawQuad(&vertices[0].x, &texcoords[0].x, tex_id);
#else
	cooking::change_shader(dlist, sl::SPRITE2);
	cooking::set_color_sprite(dlist, 0xffffffff, 0, 0x000000ff, 0x0000ff00, 0x00ff0000);
	cooking::draw_quad_sprite(dlist, &vertices[0].x, &texcoords[0].x, tex_id);
#endif // S2_DISABLE_DEFERRED

	return pt2::RENDER_OK;
}

}