#include "sprite2/Particle3dEmitter.h"
#include "sprite2/P3dRenderParams.h"
#include "sprite2/P3dEmitterCfg.h"
#include "sprite2/Particle3d.h"

#include <ps_3d.h>

#include <string.h>
#include <assert.h>

namespace s2
{

Particle3dEmitter::Particle3dEmitter()
	: m_et(0)
	, m_loop(true)
	, m_active(false)
	, m_local(true)
{
	memset(m_mt, 0, sizeof(m_mt));
	m_mt[0] = m_mt[3] = 1;
}

Particle3dEmitter::~Particle3dEmitter()
{
	if (m_et) {
		p3d_emitter_release(m_et);
		m_et = 0;
	}
}

bool Particle3dEmitter::Update(float time)
{
	if (!m_et) {
		return false;
	}

	PrepareEmitter();

	float et_time;
	p3d_emitter_get_time(m_et, &et_time);

	bool dirty = false;

	if (et_time == 0.0f)
	{
		float tot_time = Particle3d::Instance()->GetTime();
		p3d_emitter_set_time(m_et, std::min(time, tot_time));
	}
	else
	{
		if (et_time < time)
		{
			dirty = true;
			float dt = time - et_time;
			p3d_emitter_update(m_et, dt, m_mt);

			float tot_time = Particle3d::Instance()->GetTime();
			p3d_emitter_set_time(m_et, std::min(time, tot_time));
		}
	}

	return dirty;
}

bool Particle3dEmitter::PrepareEmitter()
{
	if (!m_et)
		return false;

	if (p3d_emitter_check(m_et))
		return true;

	m_et = p3d_emitter_create(m_cfg->GetImpl());
	if (!m_et)
		return false;

	if(m_active) {
		p3d_emitter_start(m_et);
	}

	p3d_emitter_set_loop(m_et, m_loop);

	return true;
}

pt2::RenderReturn Particle3dEmitter::Draw(const P3dRenderParams& rp, bool alone) const
{
	if (!m_et) {
		return pt2::RENDER_NO_DATA;
	}

	if (!const_cast<Particle3dEmitter*>(this)->PrepareEmitter()) {
		return pt2::RENDER_NO_DATA;
	}

	P3dRenderParams rp_child(rp);
	if (alone)
	{
		const float* s_mt = m_mt;
		S2_MAT d_mt;
#ifdef S2_MATRIX_FIX
		d_mt.x[0] = s_mt[0] * sm::MatrixFix::SCALE;
		d_mt.x[1] = s_mt[1] * sm::MatrixFix::SCALE;
		d_mt.x[2] = s_mt[2] * sm::MatrixFix::SCALE;
		d_mt.x[3] = s_mt[3] * sm::MatrixFix::SCALE;
		d_mt.x[4] = s_mt[4] * sm::MatrixFix::TRANSLATE_SCALE;
		d_mt.x[5] = s_mt[5] * sm::MatrixFix::TRANSLATE_SCALE;
#else
		memcpy(d_mt.x, s_mt, sizeof(d_mt.x));
#endif // S2_MATRIX_FIX
		rp_child.mt = d_mt;
	}
	rp_child.local = m_local;

	// todo: return emitter's render ret
	p3d_emitter_draw(m_et, &rp_child);

	return pt2::RENDER_OK;
}

bool Particle3dEmitter::IsLoop() const
{
	return m_loop;
}

void Particle3dEmitter::SetLoop(bool loop)
{
	m_loop = loop;
	p3d_emitter_set_loop(m_et, loop);
}

bool Particle3dEmitter::IsLocal() const
{
	return m_local;
}

void Particle3dEmitter::SetLocal(bool local)
{
	m_local = local;
}

bool Particle3dEmitter::IsFinished() const
{
	return p3d_emitter_is_finished(m_et);
}

void Particle3dEmitter::ResetTime()
{
	p3d_emitter_set_time(m_et, Particle3d::Instance()->GetTime());
}

void Particle3dEmitter::Start()
{
	m_active = true;
	p3d_emitter_set_time(m_et, Particle3d::Instance()->GetTime());
	p3d_emitter_start(m_et);
}

void Particle3dEmitter::Stop()
{
	m_active = false;
	if (m_et) {
		p3d_emitter_stop(m_et);
		p3d_emitter_clear(m_et);
	}
}

void Particle3dEmitter::Pause()
{
	m_active = false;
	if (m_et) {
		p3d_emitter_pause(m_et);
	}
}

void Particle3dEmitter::Resume()
{
	m_active = true;
	if (m_et) {
		p3d_emitter_resume(m_et);
	}
}

void Particle3dEmitter::Clear()
{
	if (m_et) {
		p3d_emitter_clear(m_et);
	}
}

float Particle3dEmitter::GetTime() const
{
	float time;
	if (!p3d_emitter_get_time(m_et, &time)) {
		return Particle3d::Instance()->GetTime();
	} else {
		return time;
	}
}

void Particle3dEmitter::SetMat(float* mat)
{
	memcpy(m_mt, mat, sizeof(m_mt));
}

void Particle3dEmitter::CreateEmitter(const std::shared_ptr<const P3dEmitterCfg>& cfg)
{
	m_cfg = cfg;
	if (m_et) {
		p3d_emitter_release(m_et);
		m_et = 0;
	}
	if (m_cfg) {
		m_et = p3d_emitter_create(m_cfg->GetImpl());
	}
}

}
