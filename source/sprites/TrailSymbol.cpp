#include "sprite2/TrailSymbol.h"
#include "sprite2/SymType.h"
#include "sprite2/TrailSprite.h"
#include "sprite2/RenderParams.h"
#include "sprite2/Sprite.h"
#include "sprite2/DrawNode.h"
#include "sprite2/TrailEmitter.h"
#include "sprite2/TrailEmitterCfg.h"
#ifndef S2_DISABLE_STATISTICS
#include "sprite2/StatSymDraw.h"
#include "sprite2/StatSymCount.h"
#endif // S2_DISABLE_STATISTICS

#include <memmgr/Allocator.h>
#include <mt_2d.h>
#include <shaderlab/Blackboard.h>
#include <shaderlab/ShaderMgr.h>
#include <shaderlab/RenderContext.h>
#include <shaderlab/Sprite2Shader.h>
#ifndef S2_DISABLE_DEFERRED
#include <cooking/DisplayList.h>
#endif // S2_DISABLE_DEFERRED

namespace s2
{

TrailSymbol::TrailSymbol()
{
#ifndef S2_DISABLE_STATISTICS
	StatSymCount::Instance()->Add(STAT_SYM_TRAIL);
#endif // S2_DISABLE_STATISTICS
}

TrailSymbol::TrailSymbol(uint32_t id)
	: Symbol(id)
{
#ifndef S2_DISABLE_STATISTICS
	StatSymCount::Instance()->Add(STAT_SYM_TRAIL);
#endif // S2_DISABLE_STATISTICS
}

TrailSymbol::~TrailSymbol()
{
#ifndef S2_DISABLE_STATISTICS
	StatSymCount::Instance()->Subtract(STAT_SYM_TRAIL);
#endif // S2_DISABLE_STATISTICS
}

int TrailSymbol::Type() const 
{ 
	return SYM_TRAIL; 
}

pt2::RenderReturn TrailSymbol::DrawTree(cooking::DisplayList* dlist, const RenderParams& rp, const Sprite* spr) const
{
#ifndef S2_DISABLE_STATISTICS
	StatSymDraw::Instance()->AddDrawCount(STAT_SYM_TRAIL);
//	StatSymDraw::DrawCostCP cp(STAT_SYM_TRAIL);
#endif // S2_DISABLE_STATISTICS

	if (!spr) {
		return pt2::RENDER_NO_DATA;
	}

	RenderParamsProxy rp_proxy;
	RenderParams* rp_child = rp_proxy.obj;
	memcpy(rp_child, &rp, sizeof(rp));

	rp_child->mt.x[4] = rp_child->mt.x[5] = 0;
	rp_child->col_common = spr->GetColorCommon() * rp.col_common;
	rp_child->col_map    = spr->GetColorMap() * rp.col_map;

	auto& shader_mgr = sl::Blackboard::Instance()->GetRenderContext().GetShaderMgr();
	sl::Sprite2Shader* shader = static_cast<sl::Sprite2Shader*>(shader_mgr.GetShader(sl::SPRITE2));
	shader->SetColor(rp_child->col_common.mul.ToABGR(), rp_child->col_common.add.ToABGR());
	shader->SetColorMap(rp_child->col_map.rmap.ToABGR(), rp_child->col_map.gmap.ToABGR(), rp_child->col_map.bmap.ToABGR());

	auto t2d_spr = S2_VI_DOWN_CAST<const TrailSprite*>(spr);
	// todo: return trail's render ret
	t2d_spr->Draw(*rp_child);

	return pt2::RENDER_OK;
}

void TrailSymbol::SetEmitterCfg(const std::shared_ptr<const TrailEmitterCfg>& cfg)
{
	if (m_et_cfg == cfg) {
		return;
	}

	m_et_cfg = std::move(cfg);
	if(!cfg) {
		return;
	}

	m_et = std::make_unique<TrailEmitter>();
	m_et->CreateEmitter(m_et_cfg);
	m_et->Start();
}

sm::rect TrailSymbol::GetBoundingImpl(const Sprite* spr, const Actor* actor, bool cache) const
{
	return sm::rect(); // empty
}

}