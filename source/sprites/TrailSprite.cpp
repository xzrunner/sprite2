#include "TrailSprite.h"
#include "TrailSymbol.h"
#include "RenderParams.h"
#include "Trail.h"

#include <mt_2d.h>

#include <assert.h>

namespace s2
{

TrailSprite::TrailSprite()
	: m_et(NULL)
	, m_local(true)
	, m_in_p3d(false)
{
}

TrailSprite::TrailSprite(const TrailSprite& spr)
	: Sprite(spr)
	, m_et(NULL)
	, m_local(spr.m_local)
	, m_in_p3d(spr.m_in_p3d)
{
	if (spr.m_et) {
		m_et = t2d_emitter_create(spr.m_et->cfg);
		t2d_emitter_start(m_et);
	}
}

TrailSprite& TrailSprite::operator = (const TrailSprite& spr)
{
	Sprite::operator = (spr);
	m_et = NULL;
	if (spr.m_et) {
		m_et = t2d_emitter_create(spr.m_et->cfg);
		t2d_emitter_start(m_et);
	}
	m_local = spr.m_local;
	m_in_p3d = spr.m_in_p3d;
	return *this;
}

TrailSprite::TrailSprite(Symbol* sym, uint32_t id)
	: Sprite(sym, id)
	, m_et(NULL)
	, m_local(true)
	, m_in_p3d(false)
{
	const t2d_emitter_cfg* cfg = VI_DOWNCASTING<TrailSymbol*>(sym)->GetEmitterCfg();
	if (cfg) {
		m_et = t2d_emitter_create(cfg);
		t2d_emitter_start(m_et);
	}
}

TrailSprite::~TrailSprite()
{
	if (m_et) {
		t2d_emitter_release(m_et);
	}
}

TrailSprite* TrailSprite::Clone() const
{
	return new TrailSprite(*this);
}

void TrailSprite::OnMessage(Message msg)
{
	switch (msg)
	{
	case MSG_START: case MSG_TRIGGER:
		if (m_et) {
			t2d_emitter_start(m_et);
		}
		break;
	default:
		break;
	}
}

bool TrailSprite::Update(const RenderParams& rp)
{
	float time = Trail::Instance()->GetTime();
	assert(m_et->time <= time);
	if (m_et->time == time) {
		return false;
	}

	float dt = time - m_et->time;
	sm::vec2 pos;
	if (m_local && !m_in_p3d) {
		pos = GetPosition();
	} else {
		pos = rp.mt * GetPosition();
	}
	t2d_emitter_update(m_et, dt, (sm_vec2*)(&pos));
	m_et->time = time;

	return true;
}

bool TrailSprite::SetFrame(int frame, const SprTreePath& parent_path, bool force)
{
	if (!force && !IsForceUpFrame() && !GetName().empty()) {
		return false;
	}
	Update(RenderParams());
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
	t2d_emitter_draw(m_et, &trp);
}

}