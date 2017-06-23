#include "Particle3dEmitter.h"
#include "P3dRenderParams.h"
#include "P3dEmitterCfg.h"
#include "Particle3d.h"

#include <ps_3d.h>

#include <string.h>
#include <assert.h>

namespace s2
{

Particle3dEmitter::Particle3dEmitter()
	: m_loop(true)
	, m_active(false)
{
	Init();
}

Particle3dEmitter::~Particle3dEmitter()
{
	Term();
}

void Particle3dEmitter::RemoveReference() const
{
	if (GetRefCount() == 1) {
		P3dEmitterPool::Instance()->Push(const_cast<Particle3dEmitter*>(this));
	} else {
		cu::RefCountObj::RemoveReference();
	}
}

bool Particle3dEmitter::Update(float time)
{
	if (!m_state.et) {
		return false;
	}

	PrepareEmitter();

	float et_time;
	p3d_emitter_get_time(m_state.et, &et_time);

	bool dirty = false;

	if (et_time == 0.0f)
	{
		float tot_time = Particle3d::Instance()->GetTime();
		p3d_emitter_set_time(m_state.et, std::min(time, tot_time));
	}
	else
	{
		if (et_time < time)
		{
			dirty = true;
			float dt = time - et_time;
			p3d_emitter_update(m_state.et, dt, m_state.mt);

			float tot_time = Particle3d::Instance()->GetTime();
			p3d_emitter_set_time(m_state.et, std::min(time, tot_time));
		}
	}

	return dirty;
}

bool Particle3dEmitter::PrepareEmitter()
{
	if (!m_state.et)
		return false;

	if (p3d_emitter_check(m_state.et))
		return true;

	m_state.et = p3d_emitter_create(m_state.cfg->GetImpl());
	if (!m_state.et)
		return false;

	if(m_active) {
		p3d_emitter_start(m_state.et);
	}

	p3d_emitter_set_loop(m_state.et, m_loop);

	return true;
}

RenderReturn Particle3dEmitter::Draw(const P3dRenderParams& rp, bool alone) const
{
	if (!m_state.et) {
		return RENDER_NO_DATA;
	}

	if (!const_cast<Particle3dEmitter*>(this)->PrepareEmitter()) {
		return RENDER_NO_DATA;
	}

	P3dRenderParams rp_child(rp);
	if (alone)
	{
		const float* s_mt = m_state.mt;
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
	rp_child.local = m_state.local;

	// todo: return emitter's render ret
	p3d_emitter_draw(m_state.et, &rp_child);

	return RENDER_OK;
}

bool Particle3dEmitter::IsLoop() const
{
	return m_loop;
}

void Particle3dEmitter::SetLoop(bool loop)
{
	m_loop = loop;
	p3d_emitter_set_loop(m_state.et, loop);
}

bool Particle3dEmitter::IsLocal() const
{
	return m_state.local;
}

void Particle3dEmitter::SetLocal(bool local)
{
	m_state.local = local;
}

bool Particle3dEmitter::IsFinished() const
{
	return p3d_emitter_is_finished(m_state.et);
}

void Particle3dEmitter::ResetTime()
{
	p3d_emitter_set_time(m_state.et, Particle3d::Instance()->GetTime());
}

void Particle3dEmitter::Start()
{
	m_active = true;
	p3d_emitter_set_time(m_state.et, Particle3d::Instance()->GetTime());
	p3d_emitter_start(m_state.et);
}

void Particle3dEmitter::Stop()
{
	m_active = false;
	if (m_state.et) {
		p3d_emitter_stop(m_state.et);
		p3d_emitter_clear(m_state.et);
	}
}

void Particle3dEmitter::Pause()
{
	m_active = false;
	if (m_state.et) {
		p3d_emitter_pause(m_state.et);
	}
}

void Particle3dEmitter::Resume()
{
	m_active = true;
	if (m_state.et) {
		p3d_emitter_resume(m_state.et);
	}
}

void Particle3dEmitter::Clear()
{
	if (m_state.et) {
		p3d_emitter_clear(m_state.et);
	}
}

float Particle3dEmitter::GetTime() const
{
	float time;
	if (!p3d_emitter_get_time(m_state.et, &time)) {
		return Particle3d::Instance()->GetTime();
	} else {
		return time;
	}
}

void Particle3dEmitter::SetMat(float* mat)
{
	memcpy(m_state.mt, mat, sizeof(m_state.mt));
}

void Particle3dEmitter::CreateEmitter(const P3dEmitterCfg* cfg)
{
	cu::RefCountObjAssign(m_state.cfg, cfg);
	if (m_state.et) {
		p3d_emitter_release(m_state.et);
		m_state.et = 0;
	}
	if (m_state.cfg) {
		m_state.et = p3d_emitter_create(m_state.cfg->GetImpl());
	}
}

void Particle3dEmitter::Init()
{
	m_state.cfg = NULL;
	m_state.et = 0;
	memset(m_state.mt, 0, sizeof(m_state.mt));
	m_state.mt[0] = m_state.mt[3] = 1;
	m_state.local = true;
}

void Particle3dEmitter::Term()
{
	if (m_state.cfg) {
		m_state.cfg->RemoveReference();
	}
	if (m_state.et) {
		p3d_emitter_release(m_state.et);
		m_state.et = 0;
	}
}

}
