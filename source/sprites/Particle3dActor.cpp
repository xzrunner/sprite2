#include "Particle3dActor.h"
#include "Particle3dSprite.h"
#include "Particle3dSymbol.h"
#include "Particle3dEmitter.h"
#include "Particle3dBuffer.h"

namespace s2
{

Particle3dActor::Particle3dActor(const Sprite* spr, const Actor* parent)
	: Actor(spr, parent) 
	, m_et(nullptr)
{
	const Particle3dSprite* p3d_spr = VI_DOWNCASTING<const Particle3dSprite*>(spr);
	const Particle3dSymbol* p3d_sym = VI_DOWNCASTING<const Particle3dSymbol*>(spr->GetSymbol());
	const P3dEmitterCfg* cfg = p3d_sym->GetEmitterCfg();
	if (cfg) 
	{
		m_et = P3dEmitterPool::Instance()->Pop();
		m_et->CreateEmitter(cfg);
		m_et->Start();

		m_et->SetLoop(p3d_spr->IsLoop());
		m_et->SetLocal(p3d_spr->IsLocal());

		if (p3d_spr->IsAlone() && m_et) {
			Particle3dBuffer::Instance()->Insert(m_et);
		}
	}
}

Particle3dActor::~Particle3dActor()
{
	const Particle3dSprite* p3d_spr = VI_DOWNCASTING<const Particle3dSprite*>(GetSpr());
	if (m_et) {
		if (p3d_spr->IsAlone()) {
			Particle3dBuffer::Instance()->Remove(m_et);
		}
		m_et->RemoveReference();
	}
}

}