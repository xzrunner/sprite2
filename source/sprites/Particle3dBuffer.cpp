#include "Particle3dBuffer.h"
#include "Particle3dEmitter.h"
#include "P3dRenderParams.h"

namespace s2
{

static void (*UPDATE_SRT_FUNC)(void* params, float x, float y, float scale);
static void (*REMOVE_FUNC)(struct Particle3dEmitter*);

Particle3dBuffer::Particle3dBuffer()
{
}

Particle3dBuffer::~Particle3dBuffer()
{
	Clear();
}

void Particle3dBuffer::Insert(Particle3dEmitter* emitter)
{
	m_emitters.push_back(emitter);
}

void Particle3dBuffer::Remove(Particle3dEmitter* emitter)
{
	std::list<Particle3dEmitter*>::iterator itr = m_emitters.begin();
	for ( ; itr != m_emitters.end(); ++itr) {
		if (*itr == emitter) {
			P3dEmitterPool::Instance()->Push(*itr);
			m_emitters.erase(itr);
			break;
		}
	}	
}

void Particle3dBuffer::Clear()
{
	std::list<Particle3dEmitter*>::iterator itr = m_emitters.begin();
	for ( ; itr != m_emitters.end(); ++itr) {
		P3dEmitterPool::Instance()->Push(*itr);
	}
	m_emitters.clear();
}

void Particle3dBuffer::Update(float time)
{
	std::list<Particle3dEmitter*>::iterator itr = m_emitters.begin();
	for ( ; itr != m_emitters.end(); ++itr) 
	{
		Particle3dEmitter* emitter = *itr;
		if (emitter->IsFinished()) {
			P3dEmitterPool::Instance()->Push(emitter);
			itr = m_emitters.erase(itr);
		} else {
			emitter->Update(time);
			++itr;
		}
	}
}

void Particle3dBuffer::Draw(const sm::vec2& pos, float scale) const
{
	std::list<Particle3dEmitter*>::const_iterator itr = m_emitters.begin();
	P3dRenderParams rp;
	for ( ; itr != m_emitters.end(); ++itr) 
	{
		rp.mt.Identity();
		rp.mt.Scale(scale, scale, scale);
		rp.mt.Translate(pos.x, pos.y, 0);
		(*itr)->Draw(rp);
	}
}

void Particle3dBuffer::InitCB(void (*update_srt_func)(void* params, float x, float y, float scale), 
							  void (*remove_func)(Particle3dEmitter*))
{
	UPDATE_SRT_FUNC = update_srt_func;
	REMOVE_FUNC = remove_func;
}

}