#include "TrailSprite.h"
#include "TrailSymbol.h"
#include "UpdateParams.h"
#include "RenderParams.h"
#include "Trail.h"
#include "S2_Actor.h"
#include "TrailEmitter.h"

#include <mt_2d.h>

#include <assert.h>

namespace s2
{

TrailSprite::TrailSprite()
	: m_et(NULL)
	, m_local(false)
	, m_in_p3d(false)
{
}

TrailSprite::TrailSprite(const TrailSprite& spr)
	: Sprite(spr)
	, m_et(NULL)
	, m_local(spr.m_local)
	, m_in_p3d(spr.m_in_p3d)
{
	CreateSpr();
}

TrailSprite& TrailSprite::operator = (const TrailSprite& spr)
{
	Sprite::operator = (spr);
	m_et             = NULL;
	m_local          = spr.m_local;
	m_in_p3d         = spr.m_in_p3d;

	CreateSpr();

	return *this;
}

TrailSprite::TrailSprite(Symbol* sym, uint32_t id)
	: Sprite(sym, id)
	, m_et(NULL)
	, m_local(false)
	, m_in_p3d(false)
{
	CreateSpr();
}

TrailSprite::~TrailSprite()
{
	if (m_et) {
		m_et->RemoveReference();
	}
}

void TrailSprite::CreateSpr()
{
	assert(!m_et);

	const TrailEmitterCfg* cfg = VI_DOWNCASTING<TrailSymbol*>(m_sym)->GetEmitterCfg();
	if (!cfg) {
		return;
	}

	m_et = TrailEmitterPool::Instance()->Pop();
	m_et->CreateEmitter(cfg);
	m_et->Start();
}

TrailSprite* TrailSprite::Clone() const
{
	return new TrailSprite(*this);
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
	const Actor* actor = up.GetActor();
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