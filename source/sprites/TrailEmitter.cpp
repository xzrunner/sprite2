#include "sprite2/TrailEmitter.h"
#include "sprite2/RenderParams.h"
#include "sprite2/TrailEmitterCfg.h"
#include "sprite2/Trail.h"

#include <mt_2d.h>

namespace s2
{

TrailEmitter::TrailEmitter()
{
	m_et = nullptr;
}

TrailEmitter::~TrailEmitter()
{
	if (m_et) {
		t2d_emitter_release(m_et);
	}
}

bool TrailEmitter::Update(float time, const sm::vec2& pos)
{
	if (!m_et) {
		return false;
	}

	bool dirty = false;

	if (m_et->time == 0)
	{
		float tot_time = Trail::Instance()->GetTime();
		m_et->time = std::min(time, tot_time);
	}
	else
	{
		if (m_et->time < time)
		{
			dirty = true;
			float dt = time - m_et->time;
			t2d_emitter_update(m_et, dt, (sm_vec2*)&pos);

			float tot_time = Trail::Instance()->GetTime();
			m_et->time = std::min(time, tot_time);
		}
	}

	return dirty;
}

void TrailEmitter::Draw(const TrailRenderParams& rp) const
{
	if (!m_et) {
		return;
	}

	t2d_emitter_draw(m_et, &rp);
}

void TrailEmitter::Start()
{
	if (m_et) {
		m_et->time = Trail::Instance()->GetTime();
		t2d_emitter_start(m_et);
	}
}

void TrailEmitter::Stop()
{
	if (m_et) {
		t2d_emitter_stop(m_et);
		t2d_emitter_clear(m_et);
	}
}

void TrailEmitter::Clear()
{
	if (m_et) {
		t2d_emitter_clear(m_et);
	}
}

float TrailEmitter::GetTime() const
{
	if (m_et) {
		return m_et->time;
	} else {
		return Trail::Instance()->GetTime();
	}
}

void TrailEmitter::CreateEmitter(const std::shared_ptr<const TrailEmitterCfg>& cfg)
{
	m_cfg = cfg;
	if (m_et) {
		t2d_emitter_release(m_et);
		m_et = nullptr;
	}
	if (m_cfg) {
		m_et = t2d_emitter_create(m_cfg->GetImpl());
	}
}

}
