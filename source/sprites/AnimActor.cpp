#include "AnimActor.h"
#include "AnimSprite.h"
#include "AnimSymbol.h"

#include <assert.h>

namespace s2
{

AnimActor::AnimActor(const SprConstPtr& spr, const ActorConstPtr& parent)
	: Actor(spr, parent)
{
}

void AnimActor::Init()
{
	assert(GetSprRaw()->HaveActor());
	m_state.Init(S2_VI_PTR_DOWN_CAST<const AnimSymbol>(GetSprRaw()->GetSymbol())->GetCopy());
}

}