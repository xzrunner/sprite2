#include "Particle3dBuffer.h"
#include "Particle3dEmitter.h"
#include "P3dRenderParams.h"

namespace s2
{

SINGLETON_DEFINITION(Particle3dBuffer)

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
	if (!emitter) {
		return;
	}

	std::list<Particle3dEmitter*>::iterator itr = m_emitters.begin();
	for ( ; itr != m_emitters.end(); ++itr) {
		if (*itr == emitter) {
			return;
		}
	}

	emitter->AddReference();
	m_emitters.push_back(emitter);
}

void Particle3dBuffer::Remove(Particle3dEmitter* emitter)
{
	std::list<Particle3dEmitter*>::iterator itr = m_emitters.begin();
	for ( ; itr != m_emitters.end(); ++itr) {
		if (*itr == emitter) {
			(*itr)->RemoveReference();
			m_emitters.erase(itr);
			break;
		}
	}	
}

void Particle3dBuffer::Clear()
{
	std::list<Particle3dEmitter*>::iterator itr = m_emitters.begin();
	for ( ; itr != m_emitters.end(); ++itr) {
		(*itr)->RemoveReference();
	}
	m_emitters.clear();
}

bool Particle3dBuffer::Update(float time)
{
	bool ret = false;
	std::list<Particle3dEmitter*>::iterator itr = m_emitters.begin();
	for ( ; itr != m_emitters.end(); ) 
	{
		Particle3dEmitter* emitter = *itr;
		if (emitter->IsFinished()) {
			(*itr)->RemoveReference();
			itr = m_emitters.erase(itr);
		} else {
			if (emitter->Update(time)) {
				ret = true;
			}
			++itr;
		}
	}
	return ret;
}

void Particle3dBuffer::Draw(const sm::vec2& pos, float scale) const
{
	std::list<Particle3dEmitter*>::const_iterator itr = m_emitters.begin();
	P3dRenderParams rp;
	for ( ; itr != m_emitters.end(); ++itr) 
	{
		rp.mt.Identity();
		rp.mt.Scale(scale, scale);
		rp.mt.Translate(pos.x, pos.y);
		(*itr)->Draw(rp, true);
	}
}

void Particle3dBuffer::InitCB(void (*update_srt_func)(void* params, float x, float y, float scale), 
							  void (*remove_func)(Particle3dEmitter*))
{
	UPDATE_SRT_FUNC = update_srt_func;
	REMOVE_FUNC = remove_func;
}

}