#include "TrailEmitter.h"
#include "RenderParams.h"
#include "TrailEmitterCfg.h"
#include "Trail.h"

#include <mt_2d.h>

namespace s2
{

TrailEmitter::TrailEmitter()
{
	Init();
}

TrailEmitter::~TrailEmitter()
{
	Term();
}

void TrailEmitter::RemoveReference() const
{
	if (GetRefCount() == 1) {
		TrailEmitterPool::Instance()->Push(const_cast<TrailEmitter*>(this));
	} else {
		cu::RefCountObj::RemoveReference();
	}
}

bool TrailEmitter::Update(float time, const sm::vec2& pos)
{
	if (!m_state.et) {
		return false;
	}

	bool dirty = false;

	if (m_state.et->time == 0)
	{
		float tot_time = Trail::Instance()->GetTime();
		m_state.et->time = std::min(time, tot_time);
	}
	else
	{
		if (m_state.et->time < time)
		{
			dirty = true;
			float dt = time - m_state.et->time;
			t2d_emitter_update(m_state.et, dt, (sm_vec2*)&pos);

			float tot_time = Trail::Instance()->GetTime();
			m_state.et->time = std::min(time, tot_time);
		}
	}

	return dirty;
}

void TrailEmitter::Draw(const TrailRenderParams& rp) const
{
	if (!m_state.et) {
		return;
	}

	t2d_emitter_draw(m_state.et, &rp);
}

void TrailEmitter::Start()
{
	if (m_state.et) {
		m_state.et->time = Trail::Instance()->GetTime();
		t2d_emitter_start(m_state.et);
	}
}

void TrailEmitter::Stop()
{
	if (m_state.et) {
		t2d_emitter_stop(m_state.et);
		t2d_emitter_clear(m_state.et);
	}
}

void TrailEmitter::Clear()
{
	if (m_state.et) {
		t2d_emitter_clear(m_state.et);
	}
}

float TrailEmitter::GetTime() const
{
	if (m_state.et) {
		return m_state.et->time;
	} else {
		return Trail::Instance()->GetTime();
	}
}

void TrailEmitter::CreateEmitter(const TrailEmitterCfg* cfg)
{
	cu::RefCountObjAssign(m_state.cfg, cfg);
	if (m_state.et) {
		t2d_emitter_release(m_state.et);
		m_state.et = nullptr;
	}
	if (m_state.cfg) {
		m_state.et = t2d_emitter_create(m_state.cfg->GetImpl());
	}
}

void TrailEmitter::Init()
{
	m_state.cfg = nullptr;
	m_state.et = nullptr;
}

void TrailEmitter::Term()
{
	if (m_state.cfg) {
		m_state.cfg->RemoveReference();
        m_state.cfg = nullptr;
	}
	if (m_state.et) {
		t2d_emitter_release(m_state.et);
		m_state.et = nullptr;
	}
}

}
