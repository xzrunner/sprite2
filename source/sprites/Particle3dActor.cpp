#include "sprite2/Particle3dActor.h"
#include "sprite2/Particle3dSprite.h"
#include "sprite2/Particle3dSymbol.h"
#include "sprite2/Particle3dEmitter.h"
#include "sprite2/Particle3dBuffer.h"

namespace s2
{

Particle3dActor::Particle3dActor(const SprConstPtr& spr, const ActorConstPtr& parent)
	: Actor(spr, parent)
{
	auto p3d_spr = S2_VI_PTR_DOWN_CAST<const Particle3dSprite>(spr);
	auto p3d_sym = S2_VI_PTR_DOWN_CAST<const Particle3dSymbol>(spr->GetSymbol());
	auto& cfg = p3d_sym->GetEmitterCfg();
	if (cfg)
	{
		m_et = mm::allocate_shared<Particle3dEmitter>();
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
	//// fixme: spr already released
	//if (m_et) {
	//	auto& p3d_spr = S2_VI_PTR_DOWN_CAST<const Particle3dSprite>(GetSprRaw());
	//	if (p3d_spr->IsAlone()) {
	//		Particle3dBuffer::Instance()->Remove(m_et);
	//	}
	//}
}

}