#include "Particle3dSymbol.h"
#include "SymType.h"
#include "Particle3dSprite.h"
#include "Particle3d.h"
#include "S2_Sprite.h"
#include "DrawNode.h"
#include "S2_Actor.h"
#include "P3dRenderParams.h"
#include "Particle3dEmitter.h"
#include "P3dEmitterCfg.h"
#include "Particle3dActor.h"
#ifndef S2_DISABLE_STATISTICS
#include "sprite2/StatTopNodes.h"
#include "sprite2/StatSymDraw.h"
#include "sprite2/StatSymCount.h"
#endif // S2_DISABLE_STATISTICS

#include <ps_3d.h>
#include <shaderlab/ShaderMgr.h>
#include <shaderlab/Sprite2Shader.h>

#include <assert.h>

namespace s2
{

Particle3dSymbol::Particle3dSymbol()
	: m_et_cfg(nullptr)
	, m_et(nullptr)
	, m_loop(true)
	, m_local(true)
{
#ifndef S2_DISABLE_STATISTICS
	StatSymCount::Instance()->Add(STAT_SYM_PARTICLE3D);
#endif // S2_DISABLE_STATISTICS

	Particle3d::Instance();
}

Particle3dSymbol::Particle3dSymbol(uint32_t id)
	: Symbol(id)
	, m_et_cfg(nullptr)
	, m_et(nullptr)
	, m_loop(true)
	, m_local(true)
{
#ifndef S2_DISABLE_STATISTICS
	StatSymCount::Instance()->Add(STAT_SYM_PARTICLE3D);
#endif // S2_DISABLE_STATISTICS

	Particle3d::Instance();
}

Particle3dSymbol::~Particle3dSymbol()
{
#ifndef S2_DISABLE_STATISTICS
	StatSymCount::Instance()->Subtract(STAT_SYM_PARTICLE3D);
#endif // S2_DISABLE_STATISTICS
}

int Particle3dSymbol::Type() const 
{ 
	return SYM_PARTICLE3D; 
}

RenderReturn Particle3dSymbol::DrawTree(const RenderParams& rp, const Sprite* spr) const
{
#ifndef S2_DISABLE_STATISTICS
	int id = -1;
	if (spr) {
		id = spr->GetSymbol()->GetID();
	}
	//StatTopNodes::Checkpoint cp(id, rp.parent_id, rp.level);
	StatSymDraw::Instance()->AddDrawCount(STAT_SYM_PARTICLE3D);
//	StatSymDraw::DrawCostCP cp2(STAT_SYM_PARTICLE3D);
#endif // S2_DISABLE_STATISTICS

	return DrawImpl(rp, spr);
}

RenderReturn Particle3dSymbol::DrawNode(cooking::DisplayList* dlist, const RenderParams& rp, const Sprite* spr, ft::FTList& ft, int pos) const
{
	return DrawImpl(rp, spr);
}

bool Particle3dSymbol::Update(const UpdateParams& up, float time)
{
// 	time = Particle3d::Instance()->GetTime();
// 	assert(m_et->time <= time);
// 	if (m_et->time == time) {
// 		return false;
// 	}
// 
// 	float mt[6];
// 	sm::mat4 inner_mat;
// 	mt[0] = inner_mat.x[0];
// 	mt[1] = inner_mat.x[1];
// 	mt[2] = inner_mat.x[4];
// 	mt[3] = inner_mat.x[5];
// 	mt[4] = inner_mat.x[12];
// 	mt[5] = inner_mat.x[13];	
// 
// 	float dt = time - m_et->time;
// 	p3d_emitter_update(m_et, dt, mt);
// 	m_et->time = time;
//	return true;

	if (m_et) {
		return m_et->Update(time);
	} else {
		return false;
	}
}

void Particle3dSymbol::SetEmitterCfg(const std::shared_ptr<const P3dEmitterCfg>& cfg)
{ 
	if (m_et_cfg == cfg) {
		return;
	}

	m_et_cfg = cfg;
	if (!cfg) {
		return;
	}

	m_et = std::make_shared<Particle3dEmitter>();
	m_et->CreateEmitter(m_et_cfg);
	m_et->Start();
}

sm::rect Particle3dSymbol::GetBoundingImpl(const Sprite* spr, const Actor* actor, bool cache) const
{
	return sm::rect(); // empty
}

RenderReturn Particle3dSymbol::DrawImpl(const RenderParams& rp, const Sprite* spr) const
{
	if (rp.IsDisableParticle3d()) {
		return RENDER_SKIP;
	}

	if (!IsVisible(rp, spr)) {
		return RENDER_INVISIBLE;
	}

	Particle3dSprite::ReuseType reuse;
	if (spr) {
		auto p3d_spr = S2_VI_DOWN_CAST<const Particle3dSprite*>(spr);
		reuse = p3d_spr->GetReuse();
	}
	else {
		reuse = Particle3dSprite::REUSE_ALL;
	}
	RenderReturn ret = RENDER_OK;
	switch (reuse)
	{
	case Particle3dSprite::REUSE_ALL:
		ret = DrawSymbol(rp, spr);
		break;
	case Particle3dSprite::REUSE_COMMON:
	{
		auto p3d_spr = S2_VI_DOWN_CAST<const Particle3dSprite*>(spr);
		ret = DrawEmitter(rp, spr, p3d_spr->GetEmitter());
	}
	break;
	case Particle3dSprite::REUSE_NONE:
	{
		if (rp.actor) {
			auto actor = S2_VI_DOWN_CAST<const Particle3dActor*>(rp.actor);
			ret = DrawEmitter(rp, spr, actor->GetEmitter());
		}
		else {
			ret = RENDER_NO_DATA;
		}
	}
	break;
	default:
		break;
	}
	return ret;
}

RenderReturn Particle3dSymbol::DrawSymbol(const RenderParams& rp, const Sprite* spr) const
{
	if (!m_et) {
		return RENDER_NO_DATA;
	}
	if (spr) {
		auto p3d_spr = S2_VI_DOWN_CAST<const Particle3dSprite*>(spr);
		if (p3d_spr->IsAlone()) {
			return RENDER_NO_DATA;
		}
	}

	RenderParamsProxy rp_proxy;
	RenderParams* rp_child = rp_proxy.obj;
	memcpy(rp_child, &rp, sizeof(rp));

#ifndef S2_DISABLE_STATISTICS
	rp_child->parent_id = spr ? spr->GetSymbol()->GetID() : -1;
	rp_child->level = rp.level + 1;
#endif // S2_DISABLE_STATISTICS
	if (!DrawNode::Prepare(rp, spr, *rp_child)) {
		return RENDER_INVISIBLE;
	}

	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
	sl::Sprite2Shader* shader = static_cast<sl::Sprite2Shader*>(mgr->GetShader(sl::SPRITE2));
	shader->SetColor(rp_child->color.GetMulABGR(), rp_child->color.GetAddABGR());
	shader->SetColorMap(rp_child->color.GetRMapABGR(), rp_child->color.GetGMapABGR(), rp_child->color.GetBMapABGR());
	P3dRenderParams p3d_rp;
	p3d_rp.mt    = rp_child->mt;
	p3d_rp.rc    = rp_child->color;
	if (spr) {
		auto p3d_spr = S2_VI_DOWN_CAST<const Particle3dSprite*>(spr);
		p3d_rp.local = p3d_spr->IsLocal();
	} else {
		p3d_rp.local = m_local;
	}
	RenderReturn ret = m_et->Draw(p3d_rp, false);

	return ret;
}

RenderReturn Particle3dSymbol::DrawEmitter(const RenderParams& rp, const Sprite* spr,
	                                       const std::shared_ptr<Particle3dEmitter>& et) const
{
	auto p3d_spr = S2_VI_DOWN_CAST<const Particle3dSprite*>(spr);

	if (p3d_spr->IsAlone() || !et) {
		return RENDER_NO_DATA;
	}

	RenderParamsProxy rp_proxy;
	RenderParams* rp_child = rp_proxy.obj;
	memcpy(rp_child, &rp, sizeof(rp));

#ifndef S2_DISABLE_STATISTICS
	rp_child->parent_id = spr ? spr->GetSymbol()->GetID() : -1;
	rp_child->level = rp.level + 1;
#endif // S2_DISABLE_STATISTICS

	rp_child->color = p3d_spr->GetColor() * rp.color;

	if (p3d_spr->IsAlone()) 
	{
// 		S2_MAT mt = p3d_spr->GetLocalMat() * rp_child->mt;
// 		if (rp.actor) {
// 			mt = rp.actor->GetLocalMat() * mt;
// 		}	
// 		p3d_spr->SetEmitterMat(mt);

//////////////////////////////////////////////////////////////////////////

//		if (!p3d) {
//			return;
//		}
//		P3dRenderParams* p3d_rp = static_cast<P3dRenderParams*>(p3d->draw_params);
//		p3d_rp->mt    = rp_child->mt;
//		p3d_rp->rc    = rp_child->color;
//		p3d_rp->local = m_local;
//		S2_MAT mt = p3d_spr->GetLocalMat() * rp_child->mt;
//		if (rp.actor) {
//			mt = rp.actor->GetLocalMat() * mt;
//		}
//#ifdef S2_MATRIX_FIX
//		p3d->mat[0] = mt.x[0] * sm::MatrixFix::SCALE;
//		p3d->mat[1] = mt.x[1] * sm::MatrixFix::SCALE;
//		p3d->mat[2] = mt.x[2] * sm::MatrixFix::SCALE;
//		p3d->mat[3] = mt.x[3] * sm::MatrixFix::SCALE;
//		p3d->mat[4] = mt.x[4] * sm::MatrixFix::TRANSLATE_SCALE_INV;
//		p3d->mat[5] = mt.x[5] * sm::MatrixFix::TRANSLATE_SCALE_INV;	
//#else
//		p3d->mat[0] = mt.x[0];
//		p3d->mat[1] = mt.x[1];
//		p3d->mat[2] = mt.x[4];
//		p3d->mat[3] = mt.x[5];
//		p3d->mat[4] = mt.x[12];
//		p3d->mat[5] = mt.x[13];
//#endif // S2_MATRIX_FIX

		return RENDER_NO_DATA;
	}

	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
	sl::Sprite2Shader* shader = static_cast<sl::Sprite2Shader*>(mgr->GetShader(sl::SPRITE2));
	shader->SetColor(rp_child->color.GetMulABGR(), rp_child->color.GetAddABGR());
	shader->SetColorMap(rp_child->color.GetRMapABGR(), rp_child->color.GetGMapABGR(), rp_child->color.GetBMapABGR());

	if (!spr->IsMatDisable() && p3d_spr->IsLocal()) {
		rp_child->mt = p3d_spr->GetLocalMat() * rp_child->mt;
		if (rp.actor) {
			rp_child->mt = rp.actor->GetLocalMat() * rp_child->mt;
		}
	}

	P3dRenderParams p3d_rp;
	p3d_rp.mt          = rp_child->mt;
	p3d_rp.rc          = rp_child->color;
	p3d_rp.local       = p3d_spr->IsLocal();
	p3d_rp.view_region = rp.GetViewRegion();
	p3d_rp.flags       = rp.GetFlags();

	RenderReturn ret = et->Draw(p3d_rp, false);

	return ret;
}

bool Particle3dSymbol::IsVisible(const RenderParams& rp, const Sprite* spr)
{
	bool visible = true;
	if (spr) {
		visible = spr->IsVisible();
	}
	if (rp.actor) {
		visible = rp.actor->IsVisible();
	}
	return visible;
}

}