#include "sprite2/Particle2dSymbol.h"
#include "sprite2/SymType.h"
#include "sprite2/Particle2dSprite.h"
#include "sprite2/RenderParams.h"
#include "sprite2/DrawNode.h"
#ifndef S2_DISABLE_STATISTICS
#include "sprite2/StatSymDraw.h"
#include "sprite2/StatSymCount.h"
#endif // S2_DISABLE_STATISTICS

#include <memmgr/Allocator.h>
#include <ps_2d.h>
#include <shaderlab/Blackboard.h>
#include <shaderlab/ShaderMgr.h>
#include <shaderlab/RenderContext.h>
#include <shaderlab/Sprite2Shader.h>
#ifndef S2_DISABLE_DEFERRED
#include <cooking/DisplayList.h>
#endif // S2_DISABLE_DEFERRED

namespace s2
{

Particle2dSymbol::Particle2dSymbol()
	: m_et_cfg(nullptr)
{
#ifndef S2_DISABLE_STATISTICS
	StatSymCount::Instance()->Add(STAT_SYM_PARTICLE2D);
#endif // S2_DISABLE_STATISTICS

	Particle2d::Instance();
}

Particle2dSymbol::Particle2dSymbol(uint32_t id)
	: Symbol(id)
	, m_et_cfg(nullptr)
{
#ifndef S2_DISABLE_STATISTICS
	StatSymCount::Instance()->Add(STAT_SYM_PARTICLE2D);
#endif // S2_DISABLE_STATISTICS

	Particle2d::Instance();
}

Particle2dSymbol::~Particle2dSymbol()
{
#ifndef S2_DISABLE_STATISTICS
	StatSymCount::Instance()->Subtract(STAT_SYM_PARTICLE2D);
#endif // S2_DISABLE_STATISTICS
}

int Particle2dSymbol::Type() const 
{ 
	return SYM_PARTICLE2D; 
}

pt2::RenderReturn Particle2dSymbol::DrawTree(cooking::DisplayList* dlist, const RenderParams& rp, const Sprite* spr) const
{
#ifndef S2_DISABLE_STATISTICS
	StatSymDraw::Instance()->AddDrawCount(STAT_SYM_PARTICLE2D);
//	StatSymDraw::DrawCostCP cp(STAT_SYM_PARTICLE2D);
#endif // S2_DISABLE_STATISTICS

	if (!spr) {
		return pt2::RENDER_NO_DATA;
	}

	RenderParamsProxy rp_proxy;
	RenderParams* rp_child = rp_proxy.obj;
	memcpy(rp_child, &rp, sizeof(rp));

	if (!DrawNode::Prepare(rp, spr, *rp_child)) {
		return pt2::RENDER_INVISIBLE;
	}

	auto p2d_spr = S2_VI_DOWN_CAST<const Particle2dSprite*>(spr);
	p2d_spr->SetMatrix(rp_child->mt);

	auto& shader_mgr = sl::Blackboard::Instance()->GetRenderContext().GetShaderMgr();
	sl::Sprite2Shader* shader = static_cast<sl::Sprite2Shader*>(shader_mgr.GetShader(sl::SPRITE2));
	shader->SetColor(rp_child->col_common.mul.ToABGR(), rp_child->col_common.add.ToABGR());
	shader->SetColorMap(rp_child->col_map.rmap.ToABGR(), rp_child->col_map.gmap.ToABGR(), rp_child->col_map.bmap.ToABGR());

	pt2::RenderReturn ret = p2d_spr->Draw(*rp_child);

	return ret;
}

#ifndef S2_DISABLE_FLATTEN
pt2::RenderReturn Particle2dSymbol::DrawNode(cooking::DisplayList* dlist, const RenderParams& rp, const Sprite* spr, ft::FTList& ft, int pos) const
{
	if (!spr) {
		return pt2::RENDER_NO_DATA;
	}

	auto p2d_spr = S2_VI_DOWN_CAST<const Particle2dSprite*>(spr);
	p2d_spr->SetMatrix(rp.mt);

	auto& shader_mgr = sl::Blackboard::Instance()->GetRenderContext().GetShaderMgr();
	sl::Sprite2Shader* shader = static_cast<sl::Sprite2Shader*>(shader_mgr.GetShader(sl::SPRITE2));
	shader->SetColor(rp.color.GetMulABGR(), rp.color.GetAddABGR());
	shader->SetColorMap(rp.color.GetRMapABGR(), rp.color.GetGMapABGR(), rp.color.GetBMapABGR());

	return p2d_spr->Draw(rp);
}
#endif // S2_DISABLE_FLATTEN

sm::rect Particle2dSymbol::GetBoundingImpl(const Sprite* spr, const Actor* actor, bool cache) const
{
	return sm::rect(); // empty
}

}