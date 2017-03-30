#include "Particle3dSymbol.h"
#include "SymType.h"
#include "Particle3dSprite.h"
#include "Particle3d.h"
#include "S2_Sprite.h"
#include "DrawNode.h"
#include "S2_Actor.h"
#include "P3dRenderParams.h"

#include <ps_3d_sprite.h>
#include <shaderlab/ShaderMgr.h>
#include <shaderlab/Sprite2Shader.h>

#include <assert.h>

namespace s2
{

Particle3dSymbol::Particle3dSymbol()
	: m_et_cfg(NULL)
	, m_et(NULL)
	, m_loop(true)
	, m_local(true)
{
	Particle3d::Instance();
}

Particle3dSymbol::Particle3dSymbol(uint32_t id)
	: Symbol(id)
	, m_et_cfg(NULL)
	, m_et(NULL)
	, m_loop(true)
	, m_local(true)
{
	Particle3d::Instance();
}

Particle3dSymbol::~Particle3dSymbol()
{
	if (!m_et) {
		return;
	}

	for (int i = 0, n = m_et->cfg->sym_count; i < n; ++i) {
		Symbol* sym = static_cast<Symbol*>(m_et->cfg->syms[i].ud);
		sym->RemoveReference();
	}

	p3d_emitter_release(m_et);
}

//bool Particle3dSymbol::Update(const UpdateParams& up)
//{
//	float time = Particle3d::Instance()->GetTime();
//	assert(m_et->time <= time);
//	if (m_et->time == time) {
//		return false;
//	}
//
//	float mt[6];
//	sm::mat4 inner_mat;
//	mt[0] = inner_mat.x[0];
//	mt[1] = inner_mat.x[1];
//	mt[2] = inner_mat.x[4];
//	mt[3] = inner_mat.x[5];
//	mt[4] = inner_mat.x[12];
//	mt[5] = inner_mat.x[13];	
//
//	float dt = time - m_et->time;
//	p3d_emitter_update(m_et, dt, mt);
//	m_et->time = time;
//
//	return true;
//}

int Particle3dSymbol::Type() const 
{ 
	return SYM_PARTICLE3D; 
}

void Particle3dSymbol::Draw(const RenderParams& rp, const Sprite* spr) const
{
	if (!spr) 
	{
		sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
		sl::Sprite2Shader* shader = static_cast<sl::Sprite2Shader*>(mgr->GetShader(sl::SPRITE2));
		shader->SetColor(rp.color.GetMulABGR(), rp.color.GetAddABGR());
		shader->SetColorMap(rp.color.GetRMapABGR(), rp.color.GetGMapABGR(), rp.color.GetBMapABGR());
		P3dRenderParams p3d_rp;
		p3d_rp.mt    = rp.mt;
		p3d_rp.rc    = rp.color;
		p3d_rp.local = m_local;
		p3d_emitter_draw(m_et, &p3d_rp);
		return;
	}

	RenderParams rp_child(rp);
	rp_child.color = spr->GetColor() * rp.color;

	const Particle3dSprite* p3d_spr = VI_DOWNCASTING<const Particle3dSprite*>(spr);
	p3d_spr->SetOuterMatrix(rp_child.mt);
	if (p3d_spr->IsAlone()) 
	{
		p3d_sprite* p3d = p3d_spr->GetP3dSpr();
		if (!p3d) {
			return;
		}
		P3dRenderParams* p3d_rp = static_cast<P3dRenderParams*>(p3d->draw_params);
		p3d_rp->mt    = rp_child.mt;
		p3d_rp->rc    = rp_child.color;
		p3d_rp->local = m_local;
		S2_MAT mt = p3d_spr->GetLocalMat() * rp_child.mt;
		if (rp.actor) {
			mt = rp.actor->GetLocalMat() * mt;
		}
#ifdef S2_MATRIX_FIX
		p3d->mat[0] = mt.x[0] * sm::MatrixFix::SCALE;
		p3d->mat[1] = mt.x[1] * sm::MatrixFix::SCALE;
		p3d->mat[2] = mt.x[2] * sm::MatrixFix::SCALE;
		p3d->mat[3] = mt.x[3] * sm::MatrixFix::SCALE;
		p3d->mat[4] = mt.x[4] * sm::MatrixFix::TRANSLATE_SCALE_INV;
		p3d->mat[5] = mt.x[5] * sm::MatrixFix::TRANSLATE_SCALE_INV;	
#else
		p3d->mat[0] = mt.x[0];
		p3d->mat[1] = mt.x[1];
		p3d->mat[2] = mt.x[4];
		p3d->mat[3] = mt.x[5];
		p3d->mat[4] = mt.x[12];
		p3d->mat[5] = mt.x[13];
#endif // S2_MATRIX_FIX
		return;
	}

	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
	sl::Sprite2Shader* shader = static_cast<sl::Sprite2Shader*>(mgr->GetShader(sl::SPRITE2));
	shader->SetColor(rp_child.color.GetMulABGR(), rp_child.color.GetAddABGR());
	shader->SetColorMap(rp_child.color.GetRMapABGR(), rp_child.color.GetGMapABGR(), rp_child.color.GetBMapABGR());

	if (p3d_spr->IsLocalModeDraw()) {
		rp_child.mt = p3d_spr->GetLocalMat() * rp_child.mt;
		if (rp.actor) {
			rp_child.mt = rp.actor->GetLocalMat() * rp_child.mt;
		}
	}

	p3d_spr->Draw(rp_child);
}

sm::rect Particle3dSymbol::GetBounding(const Sprite* spr, const Actor* actor) const
{
	// empty
	return sm::rect();
}

void Particle3dSymbol::SetEmitterCfg(p3d_emitter_cfg* cfg) 
{ 
	if (m_et_cfg == cfg) {
		return;
	}

	m_et_cfg = cfg;

	if (m_et) {
		p3d_emitter_release(m_et);
	}
	m_et = p3d_emitter_create(m_et_cfg);
	p3d_emitter_start(m_et);
}

}