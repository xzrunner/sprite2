#include "Particle3dEmitter.h"
#include "P3dRenderParams.h"

#include <ps_3d.h>

#include <string.h>
#include <assert.h>

namespace s2
{

Particle3dEmitter::Particle3dEmitter(const p3d_emitter_cfg* cfg)
{
	Init(cfg);
}

Particle3dEmitter::~Particle3dEmitter()
{
	Term();
}

bool Particle3dEmitter::Update(float time)
{
	if (!m_state.et) {
		return false;
	}

	bool dirty = false;

	if (m_state.et->time == 0) 
	{
		m_state.et->time = time;
	} 
	else 
	{
		assert(m_state.et->time <= time);
		if (m_state.et->time < time) 
		{
			dirty = true;
			float dt = time - m_state.et->time;
			p3d_emitter_update(m_state.et, dt, m_state.mt);
			m_state.et->time = time;
		}
	}

	return dirty;
}

void Particle3dEmitter::Draw(const P3dRenderParams& rp) const
{
	if (!m_state.et) {
		return;
	}

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
	d_mt.x[0]  = s_mt[0];
	d_mt.x[1]  = s_mt[1];
	d_mt.x[4]  = s_mt[2];
	d_mt.x[5]  = s_mt[3];
	d_mt.x[12] = s_mt[4];
	d_mt.x[13] = s_mt[5];
#endif // S2_MATRIX_FIX

	P3dRenderParams rp_child(rp);
	rp_child.mt = d_mt * rp.mt;
	p3d_emitter_draw(m_state.et, &rp_child);
}

bool Particle3dEmitter::IsLoop() const
{
	if (m_state.et) {
		return m_state.et->loop;
	} else {
		return false;
	}
}

void Particle3dEmitter::SetLoop(bool loop)
{
	if (m_state.et) {
		m_state.et->loop = loop;
	}
}

bool Particle3dEmitter::IsLocalModeDraw() const
{
	return m_state.local_mode_draw;
}

void Particle3dEmitter::SetLocalModeDraw(bool local)
{
	m_state.local_mode_draw = local;
}

bool Particle3dEmitter::IsFinished() const
{
	if (m_state.et) {
		return p3d_emitter_is_finished(m_state.et);
	} else {
		return true;
	}
}

void Particle3dEmitter::Start()
{
	if (m_state.et) {
		p3d_emitter_start(m_state.et);
	}
}

void Particle3dEmitter::Stop()
{
	if (m_state.et) {
		p3d_emitter_stop(m_state.et);
	}
}

void Particle3dEmitter::Init(const p3d_emitter_cfg* cfg)
{
	m_state.et = p3d_emitter_create(cfg);
	memset(m_state.mt, 0, sizeof(m_state.mt));
	m_state.local_mode_draw = true;
}

void Particle3dEmitter::Term()
{
	if (m_state.et) {
		p3d_emitter_release(m_state.et);
		m_state.et = NULL;
	}
}

}