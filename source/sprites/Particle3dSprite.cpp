#include "Particle3dSprite.h"
#include "Particle3dSymbol.h"

#include <ps_3d.h>
#include <ps_3d_sprite.h>
#include <ps_3d_buffer.h>

#include <stddef.h>
#include <assert.h>

namespace s2
{

Particle3dSprite::Particle3dSprite() 
	: m_spr(NULL)
{
}

Particle3dSprite::Particle3dSprite(const Particle3dSprite& spr)
	: Sprite(spr)
	, m_spr(NULL)
	, m_alone(spr.m_alone)
	, m_reuse(spr.m_reuse)
	, m_rp(spr.m_rp)
{
	CreateSpr();
}

Particle3dSprite& Particle3dSprite::operator = (const Particle3dSprite& spr)
{
	Sprite::operator = (spr);
	m_spr = NULL;
	m_alone = spr.m_alone;
	m_reuse = spr.m_reuse;
	m_rp = spr.m_rp;

	return *this;
}

Particle3dSprite::Particle3dSprite(Symbol* sym) 
	: Sprite(sym) 
	, m_alone(false)
	, m_reuse(false)
{
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

void Particle3dSprite::OnMessage(Message msg)
{
	if (msg == MSG_START) {
		if (m_spr) {
			p3d_emitter_start(m_spr->et);
		}
	}
}

bool Particle3dSprite::Update(const RenderParams& params)
{
	if (!m_spr) {
		return true;
	} else if (m_alone) {
		return false;
	} else {
		p3d_emitter* et = m_spr->et;

		float time = Particle3d::Instance()->GetTime();
		assert(et->time <= time);
		if (et->time == time) {
			return false;
		}

		float mt[6];
		sm::mat4 inner_mat = GetTransMatrix();
		mt[0] = inner_mat.x[0];
		mt[1] = inner_mat.x[1];
		mt[2] = inner_mat.x[4];
		mt[3] = inner_mat.x[5];
		mt[4] = inner_mat.x[12];
		mt[5] = inner_mat.x[13];	

		float dt = time - et->time;
		p3d_emitter_update(et, dt, mt);
		et->time = time;

		return true;
	}
}

void Particle3dSprite::Draw(const RenderParams& params) const
{
	if (!m_alone && m_spr) {
		m_rp.mat = params.mt;
		m_rp.ct = params.color;
		m_rp.p3d = m_spr;
		p3d_emitter_draw(m_spr->et, &m_rp);
	}
}

void Particle3dSprite::SetOuterMatrix(const sm::mat4& mat) const
{ 
	if (m_spr && m_alone) 
	{
		float* mt = m_spr->mat;
		mt[0] = mat.x[0];
		mt[1] = mat.x[1];
		mt[2] = mat.x[4];
		mt[3] = mat.x[5];
		mt[4] = mat.x[12];
		mt[5] = mat.x[13];	
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
	m_spr->local_mode_draw = sym->IsLocal();
	m_spr->et = p3d_emitter_create(cfg);
	m_spr->et->loop = sym->IsLoop();
	p3d_emitter_start(m_spr->et);
	m_spr->ptr_self = &m_spr;

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

void Particle3dSprite::SetLoop(bool loop)
{
// 	// removed from buffer
// 	if (!m_spr) {
// 		CreateSpr();
// 		p3d_buffer_insert(m_spr);
// 	}

	if (m_spr && m_spr->et) {
		m_spr->et->loop = loop;
	}
}

bool Particle3dSprite::IsLocalModeDraw() const
{
	if (m_spr) {
		return m_spr->local_mode_draw;
	} else {
		return true;
	}
}

void Particle3dSprite::SetLocalModeDraw(bool local)
{
	if (m_spr) {
		m_spr->local_mode_draw = local;
	}
}

}