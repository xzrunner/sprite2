#include "sprite2/Particle3dBuffer.h"
#include "sprite2/Particle3dEmitter.h"
#include "sprite2/P3dRenderParams.h"

namespace s2
{

CU_SINGLETON_DEFINITION(Particle3dBuffer)

static void (*UPDATE_SRT_FUNC)(void* params, float x, float y, float scale);
static void (*REMOVE_FUNC)(class Particle3dEmitter*);

Particle3dBuffer::Particle3dBuffer()
{
}

Particle3dBuffer::~Particle3dBuffer()
{
	Clear();
}

void Particle3dBuffer::Insert(const std::shared_ptr<Particle3dEmitter>& emitter)
{
	if (!emitter) {
		return;
	}

	for (auto et : m_emitters) {
		if (et.lock() == emitter) {
			return;
		}
	}

	m_emitters.push_back(emitter);
}

void Particle3dBuffer::Remove(const std::shared_ptr<Particle3dEmitter>& emitter)
{
	auto itr = m_emitters.begin();
	for ( ; itr != m_emitters.end(); ++itr) {
		if ((*itr).lock() == emitter) {
			m_emitters.erase(itr);
			break;
		}
	}	
}

void Particle3dBuffer::Clear()
{
	m_emitters.clear();
}

bool Particle3dBuffer::Update(float time)
{
	bool ret = false;
	auto itr = m_emitters.begin();
	for ( ; itr != m_emitters.end(); ) 
	{
		auto emitter = itr->lock();
		if (!emitter || emitter->IsFinished()) {
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
	P3dRenderParams rp;
	for (auto et : m_emitters)
	{
		auto et_sp = et.lock();
		if (!et_sp) {
			continue;
		}

		rp.mt.Identity();
		rp.mt.Scale(scale, scale);
		rp.mt.Translate(pos.x, pos.y);
		et_sp->Draw(rp, true);
	}
}

void Particle3dBuffer::InitCB(void (*update_srt_func)(void* params, float x, float y, float scale), 
							  void (*remove_func)(Particle3dEmitter*))
{
	UPDATE_SRT_FUNC = update_srt_func;
	REMOVE_FUNC = remove_func;
}

}