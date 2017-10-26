#include "sprite2/AnimActor.h"
#include "sprite2/AnimSprite.h"
#include "sprite2/AnimSymbol.h"

#include <assert.h>

namespace s2
{

AnimActor::AnimActor(const SprConstPtr& spr, const ActorConstPtr& parent)
	: Actor(spr, parent)
{
	m_state.Init(S2_VI_PTR_DOWN_CAST<const AnimSymbol>(GetSprRaw()->GetSymbol())->GetCopy());
}

}