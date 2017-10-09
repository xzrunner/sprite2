#include "Particle2dSprite.h"
#include "Particle2dSymbol.h"
#include "S2_Symbol.h"
#include "RenderParams.h"
#include "UpdateParams.h"
#ifndef S2_DISABLE_STATISTICS
#include "sprite2/StatSprCount.h"
#endif // S2_DISABLE_STATISTICS

#include <ps_2d.h>

#include <stddef.h>
#include <assert.h>
#include <string.h>

namespace s2
{

Particle2dSprite::Particle2dSprite() 
	: m_et(nullptr)
{
#ifndef S2_DISABLE_STATISTICS
	StatSprCount::Instance()->Add(STAT_SYM_PARTICLE2D);
#endif // S2_DISABLE_STATISTICS
}

Particle2dSprite::Particle2dSprite(const Particle2dSprite& spr)
	: Sprite(spr)
	, m_et(nullptr)
{
#ifndef S2_DISABLE_STATISTICS
	StatSprCount::Instance()->Add(STAT_SYM_PARTICLE2D);
#endif // S2_DISABLE_STATISTICS

	if (spr.m_et) {
		m_et = p2d_emitter_create(spr.m_et->cfg);
	}
}

Particle2dSprite& Particle2dSprite::operator = (const Particle2dSprite& spr)
{
	Sprite::operator = (spr);
	m_et = nullptr;
	if (spr.m_et) {
		m_et = p2d_emitter_create(spr.m_et->cfg);
	}
	return *this;
}

Particle2dSprite::Particle2dSprite(const SymPtr& sym, uint32_t id)
	: Sprite(sym, id)
	, m_et(nullptr)
{
#ifndef S2_DISABLE_STATISTICS
	StatSprCount::Instance()->Add(STAT_SYM_PARTICLE2D);
#endif // S2_DISABLE_STATISTICS

	auto cfg = S2_VI_PTR_DOWN_CAST<Particle2dSymbol>(GetSymbol())->GetEmitterCfg();
	if (cfg) {
		m_et = p2d_emitter_create(cfg);
		p2d_emitter_start(m_et);
	}
}

Particle2dSprite::~Particle2dSprite()
{
#ifndef S2_DISABLE_STATISTICS
	StatSprCount::Instance()->Subtract(STAT_SYM_PARTICLE2D);
#endif // S2_DISABLE_STATISTICS

	if (m_et) {
		p2d_emitter_release(m_et);
	}
}

void Particle2dSprite::OnMessage(const UpdateParams& up, Message msg)
{
	// update inherit
	if (!up.IsForce() && !IsInheritUpdate()) {
		return;
	}

	switch (msg)
	{
	case MSG_START: case MSG_TRIGGER:
		if (m_et) {
			p2d_emitter_start(m_et);
		}
		break;
	default:
		break;
	}
}

bool Particle2dSprite::Update(const UpdateParams& up)
{
	// update inherit
	if (!up.IsForce() && !IsInheritUpdate()) {
		return false;
	}

	// visible
	auto actor = up.GetActor();
	bool visible = actor ? actor->IsVisible() : IsVisible();
	if (!visible) {
		return false;
	}

	if (!m_et) {
		return false;
	}

	float time = Particle2d::Instance()->GetTime();
	assert(m_et->time <= time);
	if (m_et->time == time) {
		return false;
	}

	float mt[6];
#ifdef S2_MATRIX_FIX
	mt[0] = m_mat.x[0] * sm::MatrixFix::SCALE_INV;
	mt[1] = m_mat.x[1] * sm::MatrixFix::SCALE_INV;
	mt[2] = m_mat.x[2] * sm::MatrixFix::SCALE_INV;
	mt[3] = m_mat.x[3] * sm::MatrixFix::SCALE_INV;
	mt[4] = m_mat.x[4] * sm::MatrixFix::TRANSLATE_SCALE_INV;
	mt[5] = m_mat.x[5] * sm::MatrixFix::TRANSLATE_SCALE_INV;
#else
	memcpy(mt, m_mat.x, sizeof(mt));
#endif // S2_MATRIX_FIX

	float dt = time - m_et->time;
	p2d_emitter_update(m_et, dt, mt);
	m_et->time = time;

	return true;
}

RenderReturn Particle2dSprite::Draw(const RenderParams& rp) const
{
	if (!m_et) {
		return RENDER_NO_DATA;
	}
	// todo: return emitter's render ret
	if (m_et->local_mode_draw) {
		p2d_emitter_draw(m_et, &rp.mt);
	} else {
		p2d_emitter_draw(m_et, nullptr);
	}
	return RENDER_OK;
}

bool Particle2dSprite::IsLoop() const
{
	if (m_et) {
		return m_et->loop;
	} else {
		return false;
	}
}

void Particle2dSprite::SetLoop(bool loop)
{
	if (!m_et || m_et->loop == loop) {
		return;
	}

	m_et->loop = loop;
	if (m_et->loop) {
		p2d_emitter_start(m_et);
	} else {
		p2d_emitter_pause(m_et);
	}
}

bool Particle2dSprite::IsLocal() const
{
	if (m_et) {
		return m_et->local_mode_draw;
	} else {
		return false;
	}
}

void Particle2dSprite::SetLocal(bool local)
{
	if (m_et) {
		m_et->local_mode_draw = local;
	}
}

}