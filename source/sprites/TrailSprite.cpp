#include "sprite2/TrailSprite.h"
#include "sprite2/TrailSymbol.h"
#include "sprite2/UpdateParams.h"
#include "sprite2/RenderParams.h"
#include "sprite2/Trail.h"
#include "sprite2/Actor.h"
#include "sprite2/TrailEmitter.h"
#ifndef S2_DISABLE_STATISTICS
#include "sprite2/StatSprCount.h"
#endif // S2_DISABLE_STATISTICS

#include <mt_2d.h>

#include <assert.h>

namespace s2
{

TrailSprite::TrailSprite()
	: m_local(false)
	, m_in_p3d(false)
{
#ifndef S2_DISABLE_STATISTICS
	StatSprCount::Instance()->Add(STAT_SYM_TRAIL);
#endif // S2_DISABLE_STATISTICS
}

TrailSprite::TrailSprite(const TrailSprite& spr)
	: Sprite(spr)
	, m_local(spr.m_local)
	, m_in_p3d(spr.m_in_p3d)
{
#ifndef S2_DISABLE_STATISTICS
	StatSprCount::Instance()->Add(STAT_SYM_TRAIL);
#endif // S2_DISABLE_STATISTICS

	CreateSpr();
}

TrailSprite& TrailSprite::operator = (const TrailSprite& spr)
{
	Sprite::operator = (spr);
	m_et             = nullptr;
	m_local          = spr.m_local;
	m_in_p3d         = spr.m_in_p3d;

	CreateSpr();

	return *this;
}

TrailSprite::TrailSprite(const SymPtr& sym, uint32_t id)
	: Sprite(sym, id)
	, m_et(nullptr)
	, m_local(false)
	, m_in_p3d(false)
{
#ifndef S2_DISABLE_STATISTICS
	StatSprCount::Instance()->Add(STAT_SYM_TRAIL);
#endif // S2_DISABLE_STATISTICS

	CreateSpr();
}

TrailSprite::~TrailSprite()
{
#ifndef S2_DISABLE_STATISTICS
	StatSprCount::Instance()->Subtract(STAT_SYM_TRAIL);
#endif // S2_DISABLE_STATISTICS
}

void TrailSprite::CreateSpr()
{
	assert(!m_et);

	auto& cfg = S2_VI_PTR_DOWN_CAST<TrailSymbol>(GetSymbol())->GetEmitterCfg();
	if (!cfg) {
		return;
	}

	m_et = std::make_unique<TrailEmitter>();
	m_et->CreateEmitter(cfg);
	m_et->Start();
}

void TrailSprite::OnMessage(const UpdateParams& up, Message msg)
{
	switch (msg)
	{
	case MSG_START: case MSG_TRIGGER:
		if (m_et) {
			m_et->Start();
		}
		break;
	default:
		break;
	}
}

bool TrailSprite::Update(const UpdateParams& up)
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

	float time = Trail::Instance()->GetTime();
	float et_time = m_et->GetTime();
	assert(et_time <= time);
	if (et_time == time) {
		return false;
	}

	sm::vec2 pos;
	if (m_local && !m_in_p3d) {
		pos = GetPosition();
	} else {
		pos = up.GetPrevMat() * GetPosition();
	}
	m_et->Update(time, pos);

	return true;
}

void TrailSprite::Draw(const RenderParams& rp) const
{
	if (!m_et) {
		return;
	}

	TrailRenderParams trp;
	trp.ct = rp.color;
	if (m_local || m_in_p3d) {
		trp.mat = rp.mt;
	}
	m_et->Draw(trp);
}

}