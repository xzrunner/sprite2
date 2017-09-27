#include "AnimActor.h"
#include "AnimSprite.h"
#include "AnimSymbol.h"
#include "AnimCurr.h"
#include "UpdateParams.h"

#include <assert.h>

namespace s2
{

AnimActor::AnimActor(const Sprite* spr, const Actor* parent)
	: Actor(spr, parent) 
	, m_curr(nullptr)
{
	assert(spr->HaveActor());

	const AnimSymbol* anim_sym = VI_DOWNCASTING<const AnimSymbol*>(spr->GetSymbol());
	m_curr = std::make_unique<AnimCurr>();
	m_curr->SetAnimCopy(anim_sym->GetCopy());
	m_curr->Start(UpdateParams(), spr);
}

}