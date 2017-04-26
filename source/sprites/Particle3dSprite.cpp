#include "Particle3dSprite.h"
#include "Particle3dSymbol.h"
#include "Particle3dActor.h"
#include "Particle3dEmitter.h"
#include "P3dEmitterCfg.h"
#include "Particle3dBuffer.h"
#include "S2_Actor.h"
#include "UpdateParams.h"
#include "P3dRenderParams.h"
#include "RenderParams.h"
#include "CreateActorsVisitor.h"
#include "SprVisitorParams.h"

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
	Particle3dEmitter* et = NULL;
	switch (m_reuse)
	{
	case REUSE_ALL:
		{
			Symbol* sym = const_cast<Symbol*>(GetSymbol());
			et = VI_DOWNCASTING<Particle3dSymbol*>(sym)->GetEmitter();
		}
		break;
	case REUSE_COMMON:
		et = m_et;
		break;
	case REUSE_NONE:
		if (const Actor* actor = up.GetActor()) {
			const Particle3dActor* p3d_actor = static_cast<const Particle3dActor*>(actor);
			et = const_cast<Particle3dActor*>(p3d_actor)->GetEmitter();
		}
		break;
	}

	if (!et) {
		return;
	}

	switch (msg)
	{
	case MSG_START: 
		et->Start();
		et->ResetTime();
		break;
	case MSG_STOP:
		et->Stop();
		break;
	case MSG_TRIGGER:
		et->Start();
		break;
	case MSG_INIT:
		et->ResetTime();
		break;
	case MSG_CLEAR:
		et->Clear();
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
		{
			Particle3dSymbol* sym = VI_DOWNCASTING<Particle3dSymbol*>(GetSymbol());
			ret = UpdateEmitter(up, sym->GetEmitter());
		}
		break;
	case REUSE_COMMON:
		ret = UpdateEmitter(up, m_et);
		break;
	case REUSE_NONE:
		{
			const Actor* actor = up.GetActor();
			if (actor) {
				Particle3dActor* p3d_actor 
					= const_cast<Particle3dActor*>(static_cast<const Particle3dActor*>(actor));
				ret = UpdateEmitter(up, p3d_actor->GetEmitter());
			}
		}
		break;
	}
	return ret;
}

bool Particle3dSprite::SetFrame(const UpdateParams& up, int frame, bool force)
{
	if (!force && !IsInheritUpdate()) {
		return false;
	}
	Update(up);
	return true;
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
	memcpy(mt, mat.x, sizeof(mt));
#endif // S2_MATRIX_FIX

	m_et->SetMat(mt);
}

void Particle3dSprite::SetLoop(bool loop)
{
	if (m_loop == loop) {
		return;
	}

	m_loop = loop;

	// reuse all
	Symbol* sym = const_cast<Symbol*>(GetSymbol());
	if (Particle3dEmitter* et = VI_DOWNCASTING<Particle3dSymbol*>(sym)->GetEmitter()) {
		et->SetLoop(loop);
	}

	// reuse common
	if (m_et) {
		m_et->SetLoop(loop);
	}

	// reuse none
	for (int i = 0, n = m_actors.size(); i < n; ++i) {
		Particle3dActor* actor = static_cast<Particle3dActor*>(m_actors[i]);
		if (Particle3dEmitter* et = actor->GetEmitter()) {
			et->SetLoop(loop);
		}
	}
}

void Particle3dSprite::SetLocal(bool local)
{
	if (m_local == local) {
		return;
	}

	m_local = local;

	// reuse all
	Symbol* sym = const_cast<Symbol*>(GetSymbol());
	if (Particle3dEmitter* et = VI_DOWNCASTING<Particle3dSymbol*>(sym)->GetEmitter()) {
		et->SetLocal(local);
	}
	
	// reuse common
	if (m_et) {
		m_et->SetLocal(local);
	}

	// reuse none
	for (int i = 0, n = m_actors.size(); i < n; ++i) {
		Particle3dActor* actor = static_cast<Particle3dActor*>(m_actors[i]);
		if (Particle3dEmitter* et = actor->GetEmitter()) {
			et->SetLocal(local);
		}
	}
}

void Particle3dSprite::SetReuse(Particle3dSprite::ReuseType reuse)
{
	if (m_reuse == reuse) {
		return;
	}

	if (!m_alone) {
		m_reuse = reuse;
		return;
	}

	Particle3dSymbol* sym = VI_DOWNCASTING<Particle3dSymbol*>(m_sym);
	const P3dEmitterCfg* cfg = sym->GetEmitterCfg();
	if (!cfg) {
		return;
	}

	ChangeEmitterBuffer(m_reuse, cfg, false);
	if (m_reuse == REUSE_NONE) {
		ClearActors();
	}
	if (reuse == REUSE_NONE) {
		SetNeedActor(true);
		CreateActorsVisitor visitor;
		SprVisitorParams vp;
		Traverse(visitor, vp);
	}
	ChangeEmitterBuffer(reuse, cfg, true);

	m_reuse = reuse;
}

void Particle3dSprite::SetAlone(bool alone) 
{ 
	if (m_alone == alone) {
		return;
	}

	Particle3dSymbol* sym = VI_DOWNCASTING<Particle3dSymbol*>(m_sym);
	const P3dEmitterCfg* cfg = sym->GetEmitterCfg();
	if (!cfg) {
		return;
	}

	m_alone = alone;
	ChangeEmitterBuffer(m_reuse, cfg, m_alone);
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

bool Particle3dSprite::UpdateEmitter(const UpdateParams& up, Particle3dEmitter* et)
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
	memcpy(mt, world_mat.x, sizeof(mt));
#endif // S2_MATRIX_FIX

	et->SetMat(mt);
	if (m_alone) {
		return false;
	}

	float time = Particle3d::Instance()->GetTime();
	float et_time = et->GetTime();
	assert(et_time <= time);
	if (et_time == time) {
		return false;
	}

	et->Update(time);

	return true;
}

void Particle3dSprite::ChangeEmitterBuffer(ReuseType reuse, const P3dEmitterCfg* cfg, bool add)
{
	switch (reuse)
	{
	case REUSE_ALL:
		{
			Particle3dSymbol* sym = VI_DOWNCASTING<Particle3dSymbol*>(m_sym);
			Particle3dEmitter* et = VI_DOWNCASTING<Particle3dSymbol*>(sym)->GetEmitter();
			ChangeEmitterBuffer(et, cfg, add);
		}
		break;
	case REUSE_COMMON:
		ChangeEmitterBuffer(m_et, cfg, add);
		break;
	case REUSE_NONE:
		for (int i = 0, n = m_actors.size(); i < n; ++i) {
			Particle3dActor* actor = static_cast<Particle3dActor*>(m_actors[i]);
			Particle3dEmitter* et = actor->GetEmitter();
			ChangeEmitterBuffer(et, cfg, add);			
		}
		break;
	}
}

void Particle3dSprite::ChangeEmitterBuffer(Particle3dEmitter* et, const P3dEmitterCfg* cfg, bool add)
{
	if (!et) {
		return;
	}

	et->Clear();
	et->CreateEmitter(cfg);
	if (add) {
		Particle3dBuffer::Instance()->Insert(et);
	} else {
		Particle3dBuffer::Instance()->Remove(et);
	}
	et->Start();
}

}