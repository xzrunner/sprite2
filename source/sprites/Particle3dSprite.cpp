#include "Particle3dSprite.h"
#include "Particle3dSymbol.h"
#include "Particle3dEmitter.h"
#include "P3dEmitterCfg.h"
#include "Particle3dBuffer.h"
#include "S2_Actor.h"
#include "UpdateParams.h"
#include "P3dRenderParams.h"
#include "RenderParams.h"

#include <ps_3d.h>

#include <stddef.h>
#include <assert.h>

namespace s2
{

Particle3dSprite::Particle3dSprite() 
	: m_et(NULL)
	, m_loop(true)
	, m_local(true)
	, m_alone(false)
	, m_reuse(REUSE_COMMON)
	, m_start_radius(FLT_MAX)
{
}

Particle3dSprite::Particle3dSprite(const Particle3dSprite& spr)
	: Sprite(spr)
	, m_et(NULL)
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
	m_et             = NULL;
	m_loop           = spr.m_loop;
	m_local          = spr.m_local;
	m_alone          = spr.m_alone;
	m_reuse          = spr.m_reuse;
	m_start_radius   = spr.m_start_radius;

	return *this;
}

Particle3dSprite::Particle3dSprite(Symbol* sym, uint32_t id) 
	: Sprite(sym, id)
	, m_et(NULL)
	, m_alone(false)
	, m_reuse(REUSE_COMMON)
	, m_start_radius(FLT_MAX)
{
	Particle3dSymbol* p3d_sym = VI_DOWNCASTING<Particle3dSymbol*>(m_sym);
	m_loop  = p3d_sym->IsLoop();
	m_local = p3d_sym->IsLocal();

	CreateSpr();
}

Particle3dSprite::~Particle3dSprite()
{
	if (m_et) {
		m_et->RemoveReference();
	}
}

Particle3dSprite* Particle3dSprite::Clone() const
{
	return new Particle3dSprite(*this);
}

void Particle3dSprite::OnMessage(const UpdateParams& up, Message msg)
{
	if (!m_et) {
		return;
	}

	switch (msg)
	{
	case MSG_START: case MSG_TRIGGER:
		m_et->Start();
		break;
	case MSG_INIT:
		m_et->ResetTime();
		break;
	}
}

bool Particle3dSprite::Update(const UpdateParams& up)
{
	if (!m_et) {
		return false;
	}

	bool ret = false;
	switch (m_reuse)
	{
	case REUSE_ALL:
		ret = UpdateSymbol(up);
		break;
	case REUSE_COMMON:
		ret = UpdateSprite(up);
		break;
	case REUSE_NONE:
		ret = UpdateActor(up);
		break;
	}
	return ret;
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
	if (m_alone || !m_et) {
		return;
	}

	P3dRenderParams p3d_rp;
	p3d_rp.mt          = rp.mt;
	p3d_rp.rc          = rp.color;
	p3d_rp.local       = m_local;
	p3d_rp.view_region = rp.view_region;
	m_et->Draw(p3d_rp, false);
}

void Particle3dSprite::SetEmitterMat(const S2_MAT& mat) const
{
	if (!m_et || !m_alone) {
		return;
	}

	float mt[6];
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

	m_et->SetMat(mt);
}

void Particle3dSprite::CreateSpr()
{
	assert(!m_et);

	Particle3dSymbol* sym = VI_DOWNCASTING<Particle3dSymbol*>(m_sym);
	const P3dEmitterCfg* cfg = sym->GetEmitterCfg();
	if (!cfg) {
		return;
	}

	m_et = P3dEmitterPool::Instance()->Pop();
	m_et->CreateEmitter(cfg);
	m_et->Start();

	m_et->SetLoop(m_loop);
	m_et->SetLocal(m_local);
	
	if (m_start_radius == FLT_MAX) {
		m_start_radius = cfg->GetImpl()->start_radius;
	}

	if (m_alone && m_et) {
		Particle3dBuffer::Instance()->Insert(m_et);
	}
}

void Particle3dSprite::SetLoop(bool loop)
{
	if (m_loop == loop || !m_et) {
		return;
	}

	if (m_alone && loop) {
		return;
	}

	m_loop = loop;
	m_et->SetLoop(loop);
}

void Particle3dSprite::SetLocal(bool local)
{
	if (m_local == local || !m_et) {
		return;
	}

	m_local = local;
	m_et->SetLocal(local);
}

void Particle3dSprite::SetReuse(Particle3dSprite::ReuseType reuse)
{
	if (m_reuse == reuse || !m_et) {
		return;
	}

	m_reuse = reuse;
}

void Particle3dSprite::SetAlone(bool alone) 
{ 
	if (m_alone == alone || !m_et) {
		return;
	}

	m_et->Clear();

	Particle3dSymbol* sym = VI_DOWNCASTING<Particle3dSymbol*>(m_sym);
	const P3dEmitterCfg* cfg = sym->GetEmitterCfg();
	if (!cfg) {
		return;
	}

	m_et->CreateEmitter(cfg);
	if (m_alone) {
		Particle3dBuffer::Instance()->Remove(m_et);
	} else {
		Particle3dBuffer::Instance()->Insert(m_et);
	}
	m_et->Start();

	m_alone = alone;
}

bool Particle3dSprite::UpdateSymbol(const UpdateParams& up)
{
	float time = Particle3d::Instance()->GetTime();
	return m_sym->Update(up, time);
}

bool Particle3dSprite::UpdateSprite(const UpdateParams& up)
{
	Particle3dSymbol* sym = VI_DOWNCASTING<Particle3dSymbol*>(m_sym);
	const P3dEmitterCfg* cfg = sym->GetEmitterCfg();
	if (!cfg) {
		return false;
	}

	const_cast<P3dEmitterCfg*>(cfg)->SetStartRadius(m_start_radius);

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

	m_et->SetMat(mt);
	if (m_alone) {
		return false;
	}

	float time = Particle3d::Instance()->GetTime();
	float et_time = m_et->GetTime();
	assert(et_time <= time);
	if (et_time == time) {
		return false;
	}

	m_et->Update(time);

	return true;
}

bool Particle3dSprite::UpdateActor(const UpdateParams& up)
{
	return false;
}

}