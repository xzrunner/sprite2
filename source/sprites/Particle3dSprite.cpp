#include "Particle3dSprite.h"
#include "Particle3dSymbol.h"
#include "S2_Actor.h"
#include "UpdateParams.h"
#include "P3dRenderParams.h"
#include "RenderParams.h"

#include <ps_3d.h>
#include <ps_3d_sprite.h>
#include <ps_3d_buffer.h>

#include <stddef.h>
#include <assert.h>

namespace s2
{

Particle3dSprite::Particle3dSprite() 
	: m_spr(NULL)
	, m_loop(true)
	, m_local(true)
	, m_alone(false)
	, m_reuse(false)
	, m_start_radius(FLT_MAX)
{
}

Particle3dSprite::Particle3dSprite(const Particle3dSprite& spr)
	: Sprite(spr)
	, m_spr(NULL)
	, m_loop(spr.m_loop)
	, m_local(spr.m_local)
	, m_alone(spr.m_alone)
	, m_reuse(spr.m_reuse)
	, m_start_radius(spr.m_start_radius)
{
	CreateSpr();
}

Particle3dSprite& Particle3dSprite::operator = (const Particle3dSprite& spr)
{
	Sprite::operator = (spr);
	m_spr            = NULL;
	m_loop           = spr.m_loop;
	m_local          = spr.m_local;
	m_alone          = spr.m_alone;
	m_reuse          = spr.m_reuse;
	m_start_radius   = spr.m_start_radius;

	return *this;
}

Particle3dSprite::Particle3dSprite(Symbol* sym, uint32_t id) 
	: Sprite(sym, id)
	, m_alone(false)
	, m_reuse(false)
	, m_start_radius(FLT_MAX)
{
	Particle3dSymbol* p3d_sym = VI_DOWNCASTING<Particle3dSymbol*>(m_sym);
	m_loop  = p3d_sym->IsLoop();
	m_local = p3d_sym->IsLocal();

	CreateSpr();
}

Particle3dSprite::~Particle3dSprite()
{
	if (!m_spr) {
		return;
	}

	if (m_spr->et->loop) {
		p3d_buffer_remove(m_spr);
	}
	p3d_sprite_release(m_spr);
}

Particle3dSprite* Particle3dSprite::Clone() const
{
	return new Particle3dSprite(*this);
}

void Particle3dSprite::OnMessage(const UpdateParams& up, Message msg)
{
	if (!m_spr) {
		return;
	}

	switch (msg)
	{
	case MSG_START: case MSG_TRIGGER:
		p3d_emitter_start(m_spr->et);
		break;
	case MSG_INIT:
		if (m_spr->et->time == 0) {
			m_spr->et->time = Particle3d::Instance()->GetTime();
		}
		break;
	}
}

bool Particle3dSprite::Update(const UpdateParams& up)
{
	if (!m_spr) {
		return false;
	} 
	
	// update outside
	if (m_alone) {
		p3d_emitter* et = m_spr->et;

		p3d_emitter_cfg* cfg = const_cast<p3d_emitter_cfg*>(et->cfg);
		cfg->start_radius = m_start_radius;

		return false;
	} 

	p3d_emitter* et = m_spr->et;

	p3d_emitter_cfg* cfg = const_cast<p3d_emitter_cfg*>(et->cfg);
	cfg->start_radius = m_start_radius;

	float time = Particle3d::Instance()->GetTime();
	assert(et->time <= time);
	if (et->time == time) {
		return false;
	}

	UpdateParams up_child(up);
	up_child.Push(this);

	float mt[6];
	const S2_MAT& world_mat = up_child.GetPrevMat();
#ifdef S2_MATRIX_FIX
	mt[0] = world_mat.x[0] * sm::MatrixFix::SCALE_INV;
	mt[1] = world_mat.x[1] * sm::MatrixFix::SCALE_INV;
	mt[2] = world_mat.x[2] * sm::MatrixFix::SCALE_INV;
	mt[3] = world_mat.x[3] * sm::MatrixFix::SCALE_INV;
	mt[4] = world_mat.x[4] * sm::MatrixFix::TRANSLATE_SCALE_INV;
	mt[5] = world_mat.x[5] * sm::MatrixFix::TRANSLATE_SCALE_INV;
#else
	mt[0] = world_mat.x[0];
	mt[1] = world_mat.x[1];
	mt[2] = world_mat.x[4];
	mt[3] = world_mat.x[5];
	mt[4] = world_mat.x[12];
	mt[5] = world_mat.x[13];
#endif // S2_MATRIX_FIX
	
	float dt = time - et->time;
	p3d_emitter_update(et, dt, mt);
	et->time = time;

	return true;
}

bool Particle3dSprite::SetFrame(const UpdateParams& up, int frame, bool force)
{
	if (!force && !ShouldInheritFrame()) {
		return false;
	}
	Update(up);
	return true;
}

void Particle3dSprite::Draw(const RenderParams& rp) const
{
	if (m_alone || !m_spr) {
		return;
	}

	P3dRenderParams p3d_rp;
	p3d_rp.mt          = rp.mt;
	p3d_rp.rc          = rp.color;
	p3d_rp.local       = m_spr->local_mode_draw;
	p3d_rp.view_region = rp.view_region;
	p3d_emitter_draw(m_spr->et, &p3d_rp);
}

void Particle3dSprite::SetOuterMatrix(const S2_MAT& mat) const
{ 
	if (m_spr && m_alone) 
	{
		float* mt = m_spr->mat;
#ifdef S2_MATRIX_FIX
		mt[0] = mat.x[0] * sm::MatrixFix::SCALE_INV;
		mt[1] = mat.x[1] * sm::MatrixFix::SCALE_INV;
		mt[2] = mat.x[2] * sm::MatrixFix::SCALE_INV;
		mt[3] = mat.x[3] * sm::MatrixFix::SCALE_INV;
		mt[4] = mat.x[4] * sm::MatrixFix::TRANSLATE_SCALE_INV;
		mt[5] = mat.x[5] * sm::MatrixFix::TRANSLATE_SCALE_INV;	
#else
		mt[0] = mat.x[0];
		mt[1] = mat.x[1];
		mt[2] = mat.x[4];
		mt[3] = mat.x[5];
		mt[4] = mat.x[12];
		mt[5] = mat.x[13];	
#endif // S2_MATRIX_FIX
	}
}

void Particle3dSprite::CreateSpr()
{
	Particle3dSymbol* sym = VI_DOWNCASTING<Particle3dSymbol*>(m_sym);
	const p3d_emitter_cfg* cfg = sym->GetEmitterCfg();
	if (!cfg) {
		return;
	}

	m_spr = p3d_sprite_create();
	m_spr->local_mode_draw = m_local;
	m_spr->et              = p3d_emitter_create(cfg);
	m_spr->et->loop        = m_loop;
	p3d_emitter_start(m_spr->et);
	m_spr->ptr_self        = &m_spr;
	if (m_start_radius == FLT_MAX) {
		m_start_radius = cfg->start_radius;
	}

	if (m_alone && m_spr) {
		p3d_buffer_insert(m_spr);
	}
}

void Particle3dSprite::SetAlone(bool alone) 
{ 
	if (m_alone == alone || !m_spr) {
		return;
	}

	if (m_spr->et) {
		p3d_emitter_clear(m_spr->et);
	}

	const p3d_emitter_cfg* cfg = m_spr->et->cfg;
	p3d_emitter* et = p3d_emitter_create(cfg);
	if (m_alone) {
		p3d_buffer_remove(m_spr);
	} else {
		p3d_buffer_insert(m_spr);
	}
	m_spr->et = et;
	p3d_emitter_start(m_spr->et);

	m_alone = alone;
}

void Particle3dSprite::SetReuse(bool reuse)
{
	if (m_reuse == reuse || !m_spr) {
		return;
	}

	if (m_spr->et) {
		p3d_emitter_release(m_spr->et);
		m_spr->et = NULL;
	}

	Particle3dSymbol* sym = VI_DOWNCASTING<Particle3dSymbol*>(m_sym);
	if (m_reuse) {
		m_spr->et = sym->GetEmitter();
	} else {
		m_spr->et = p3d_emitter_create(sym->GetEmitterCfg());
	}
	p3d_emitter_start(m_spr->et);

	m_reuse = reuse;
}

}