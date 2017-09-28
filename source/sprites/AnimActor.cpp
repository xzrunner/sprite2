#include "AnimActor.h"
#include "AnimSprite.h"
#include "AnimSymbol.h"

#include <assert.h>

namespace s2
{

AnimActor::AnimActor(const Sprite* spr, const Actor* parent)
	: Actor(spr, parent) 
{
	assert(spr->HaveActor());
	const AnimSymbol* anim_sym = VI_DOWNCASTING<const AnimSymbol*>(spr->GetSymbol());
	m_state.Init(anim_sym->GetCopy(), spr);
}

}