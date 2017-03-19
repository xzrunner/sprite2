#include "AnimActor.h"
#include "AnimSprite.h"
#include "AnimCurr.h"
#include "AnimSymbol.h"
#include "S2_Sprite.h"

#include <assert.h>

namespace s2
{

AnimActor::AnimActor(const Sprite* spr, const SprTreePath& path)
	: Actor(spr, path) 
	, m_curr(NULL)
{
	assert(spr->HaveActor());
	if (spr->ActorCount() == 1) {
		const AnimSprite* anim_spr = VI_DOWNCASTING<const AnimSprite*>(spr);
		const AnimCurr& curr = anim_spr->GetAnimCurr(path);
		curr.AddReference();
		m_curr = const_cast<AnimCurr*>(&curr);
	} else {
		m_curr = new AnimCurr();
	}

	const AnimSymbol* anim_sym = VI_DOWNCASTING<const AnimSymbol*>(spr->GetSymbol());
	m_curr->SetAnimCopy(&const_cast<AnimSymbol*>(anim_sym)->GetCopy());
	m_curr->Start(path);
}

AnimActor::~AnimActor()
{
	if (m_curr) {
		m_curr->RemoveReference();
	}
}

}