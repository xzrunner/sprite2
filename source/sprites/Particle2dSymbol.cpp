#include "Particle2dSymbol.h"
#include "SymType.h"
#include "Particle2dSprite.h"
#include "RenderParams.h"
#include "DrawNode.h"
#ifndef S2_DISABLE_STATISTICS
#include "sprite2/StatSymDraw.h"
#include "sprite2/StatSymCount.h"
#endif // S2_DISABLE_STATISTICS

#include <ps_2d.h>
#include <shaderlab/ShaderMgr.h>
#include <shaderlab/Sprite2Shader.h>

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

RenderReturn Particle2dSymbol::DrawTree(const RenderParams& rp, const Sprite* spr) const
{
#ifndef S2_DISABLE_STATISTICS
	StatSymDraw::Instance()->AddDrawCount(STAT_SYM_PARTICLE2D);
//	StatSymDraw::DrawCostCP cp(STAT_SYM_PARTICLE2D);
#endif // S2_DISABLE_STATISTICS

	if (!spr) {
		return RENDER_NO_DATA;
	}

	RenderParams* rp_child = RenderParamsPool::Instance()->Pop();
	*rp_child = rp;
	if (!DrawNode::Prepare(rp, spr, *rp_child)) {
		RenderParamsPool::Instance()->Push(rp_child); 
		return RENDER_INVISIBLE;
	}

	const Particle2dSprite* p2d_spr = VI_DOWNCASTING<const Particle2dSprite*>(spr);
	p2d_spr->SetMatrix(rp_child->mt);

	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
	sl::Sprite2Shader* shader = static_cast<sl::Sprite2Shader*>(mgr->GetShader(sl::SPRITE2));
	shader->SetColor(rp_child->color.GetMulABGR(), rp_child->color.GetAddABGR());
	shader->SetColorMap(rp_child->color.GetRMapABGR(), rp_child->color.GetGMapABGR(), rp_child->color.GetBMapABGR());

	RenderReturn ret = p2d_spr->Draw(*rp_child);

	RenderParamsPool::Instance()->Push(rp_child); 

	return ret;
}

RenderReturn Particle2dSymbol::DrawNode(cooking::DisplayList* dlist, const RenderParams& rp, const Sprite* spr, ft::FTList& ft, int pos) const
{
	if (!spr) {
		return RENDER_NO_DATA;
	}

	const Particle2dSprite* p2d_spr = VI_DOWNCASTING<const Particle2dSprite*>(spr);
	p2d_spr->SetMatrix(rp.mt);

	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
	sl::Sprite2Shader* shader = static_cast<sl::Sprite2Shader*>(mgr->GetShader(sl::SPRITE2));
	shader->SetColor(rp.color.GetMulABGR(), rp.color.GetAddABGR());
	shader->SetColorMap(rp.color.GetRMapABGR(), rp.color.GetGMapABGR(), rp.color.GetBMapABGR());

	return p2d_spr->Draw(rp);
}

sm::rect Particle2dSymbol::GetBoundingImpl(const Sprite* spr, const Actor* actor, bool cache) const
{
	return sm::rect(); // empty
}

}