#include "sprite2/Particle3dSprite.h"
#include "sprite2/Particle3dSymbol.h"
#include "sprite2/Particle3dActor.h"
#include "sprite2/Particle3dEmitter.h"
#include "sprite2/P3dEmitterCfg.h"
#include "sprite2/Particle3dBuffer.h"
#include "sprite2/Actor.h"
#include "sprite2/UpdateParams.h"
#include "sprite2/P3dRenderParams.h"
#include "sprite2/RenderParams.h"
#include "sprite2/CreateActorsVisitor.h"
#include "sprite2/SprVisitorParams.h"
#ifndef S2_DISABLE_STATISTICS
#include "sprite2/StatSprCount.h"
#endif // S2_DISABLE_STATISTICS

#include <ps_3d.h>

#include <stddef.h>
#include <assert.h>
#include <string.h>

namespace s2
{

Particle3dSprite::Particle3dSprite() 
	: m_et(nullptr)
	, m_loop(true)
	, m_local(true)
	, m_alone(false)
	, m_reuse(REUSE_COMMON)
	, m_start_radius(FLT_MAX)
{
#ifndef S2_DISABLE_STATISTICS
	StatSprCount::Instance()->Add(STAT_SYM_PARTICLE3D);
#endif // S2_DISABLE_STATISTICS
}

Particle3dSprite::Particle3dSprite(const Particle3dSprite& spr)
	: Sprite(spr)
	, m_et(nullptr)
	, m_loop(spr.m_loop)
	, m_local(spr.m_local)
	, m_alone(spr.m_alone)
	, m_reuse(spr.m_reuse)
	, m_start_radius(spr.m_start_radius)
{
#ifndef S2_DISABLE_STATISTICS
	StatSprCount::Instance()->Add(STAT_SYM_PARTICLE3D);
#endif // S2_DISABLE_STATISTICS

	CreateSpr();
}

Particle3dSprite& Particle3dSprite::operator = (const Particle3dSprite& spr)
{
	Sprite::operator = (spr);
	m_et             = nullptr;
	m_loop           = spr.m_loop;
	m_local          = spr.m_local;
	m_alone          = spr.m_alone;
	m_reuse          = spr.m_reuse;
	m_start_radius   = spr.m_start_radius;

	CreateSpr();

	return *this;
}

Particle3dSprite::Particle3dSprite(const SymPtr& sym, uint32_t id)
	: Sprite(sym, id)
	, m_et(nullptr)
	, m_alone(false)
	, m_reuse(REUSE_COMMON)
	, m_start_radius(FLT_MAX)
{
#ifndef S2_DISABLE_STATISTICS
	StatSprCount::Instance()->Add(STAT_SYM_PARTICLE3D);
#endif // S2_DISABLE_STATISTICS

	auto p3d_sym = S2_VI_PTR_DOWN_CAST<Particle3dSymbol>(m_sym);
	m_loop  = p3d_sym->IsLoop();
	m_local = p3d_sym->IsLocal();

	CreateSpr();
}

Particle3dSprite::~Particle3dSprite()
{
#ifndef S2_DISABLE_STATISTICS
	StatSprCount::Instance()->Subtract(STAT_SYM_PARTICLE3D);
#endif // S2_DISABLE_STATISTICS
}

void Particle3dSprite::OnMessage(const UpdateParams& up, Message msg)
{
	// update inherit
	if (!up.IsForce() && !IsInheritUpdate()) {
		return;
	}

	std::shared_ptr<Particle3dEmitter> et;
	switch (m_reuse)
	{
	case REUSE_ALL:
		et = S2_VI_PTR_DOWN_CAST<Particle3dSymbol>(GetSymbol())->GetEmitter();
		break;
	case REUSE_COMMON:
		et = m_et;
		break;
	case REUSE_NONE:
		if (auto actor = up.GetActor()) {
			et = S2_VI_DOWN_CAST<const Particle3dActor*>(actor)->GetEmitter();
		}
		break;
	}

	if (!et) {
		return;
	}

	switch (msg)
	{
	case MSG_START: case MSG_PLAY:
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

	bool ret = false;
	switch (m_reuse)
	{
	case REUSE_ALL:
		{
			auto& et = S2_VI_PTR_DOWN_CAST<Particle3dSymbol>(GetSymbol())->GetEmitter();
			ret = UpdateEmitter(up, et);
		}
		break;
	case REUSE_COMMON:
		ret = UpdateEmitter(up, m_et);
		break;
	case REUSE_NONE:
		{
			auto actor = up.GetActor();
			if (actor) {
				auto a = S2_VI_DOWN_CAST<const Particle3dActor*>(actor);
				auto ac = const_cast<Particle3dActor*>(a);
				ret = UpdateEmitter(up, ac->GetEmitter());
			}
		}
		break;
	}
	return ret;
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
	if (auto& et = S2_VI_PTR_DOWN_CAST<Particle3dSymbol>(GetSymbol())->GetEmitter()) {
		et->SetLoop(loop);
	}

	// reuse common
	if (m_et) {
		m_et->SetLoop(loop);
	}

	// reuse none
	if (m_actors)
	{
		auto& actors = m_actors->GetActors();
		for (auto& actor : actors) 
		{
			if (!actor) {
				continue;
			}
			if (auto& et = std::static_pointer_cast<Particle3dActor>(actor)->GetEmitter()) {
				et->SetLoop(loop);
			}
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
	if (auto& et = S2_VI_PTR_DOWN_CAST<Particle3dSymbol>(GetSymbol())->GetEmitter()) {
		et->SetLocal(local);
	}
	
	// reuse common
	if (m_et) {
		m_et->SetLocal(local);
	}

	// reuse none
	if (m_actors)
	{
		auto& actors = m_actors->GetActors();
		for (auto& actor : actors) 
		{
			if (!actor) {
				continue;
			}
			if (auto& et = std::static_pointer_cast<Particle3dActor>(actor)->GetEmitter()) {
				et->SetLocal(local);
			}
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

	auto& cfg = S2_VI_PTR_DOWN_CAST<Particle3dSymbol>(GetSymbol())->GetEmitterCfg();
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

	auto& cfg = S2_VI_PTR_DOWN_CAST<Particle3dSymbol>(GetSymbol())->GetEmitterCfg();
	if (!cfg) {
		return;
	}

	m_alone = alone;
	ChangeEmitterBuffer(m_reuse, cfg, m_alone);
}

void Particle3dSprite::CreateSpr()
{
	assert(!m_et);

	auto& cfg = S2_VI_PTR_DOWN_CAST<Particle3dSymbol>(GetSymbol())->GetEmitterCfg();
	if (!cfg) {
		return;
	}

	m_et = mm::allocate_shared<Particle3dEmitter>();
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

bool Particle3dSprite::UpdateEmitter(const UpdateParams& up, const std::shared_ptr<Particle3dEmitter>& et)
{
	auto& cfg = S2_VI_PTR_DOWN_CAST<Particle3dSymbol>(GetSymbol())->GetEmitterCfg();
	if (!cfg) {
		return false;
	}

	std::const_pointer_cast<P3dEmitterCfg, const P3dEmitterCfg>(cfg)->SetStartRadius(m_start_radius);

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

	et->PrepareEmitter();

	float time = Particle3d::Instance()->GetTime();
	float et_time = et->GetTime();
	assert(et_time <= time);
	if (et_time == time) {
		return false;
	}

	et->Update(time);

	return true;
}

void Particle3dSprite::ChangeEmitterBuffer(ReuseType reuse, const std::shared_ptr<const P3dEmitterCfg>& cfg, bool add)
{
	switch (reuse)
	{
	case REUSE_ALL:
		{
			auto& et = S2_VI_PTR_DOWN_CAST<Particle3dSymbol>(GetSymbol())->GetEmitter();
			ChangeEmitterBuffer(et, cfg, add);
		}
		break;
	case REUSE_COMMON:
		ChangeEmitterBuffer(m_et, cfg, add);
		break;
	case REUSE_NONE:
		if (m_actors)
		{
			auto& actors = m_actors->GetActors();
			for (auto& actor : actors) 
			{
				if (!actor) {
					continue;
				}
				if (auto& et = std::static_pointer_cast<Particle3dActor>(actor)->GetEmitter()) {
					ChangeEmitterBuffer(et, cfg, add);
				}
			}
		}
		break;
	}
}

void Particle3dSprite::ChangeEmitterBuffer(std::shared_ptr<Particle3dEmitter>& et, const std::shared_ptr<const P3dEmitterCfg>& cfg, bool add)
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