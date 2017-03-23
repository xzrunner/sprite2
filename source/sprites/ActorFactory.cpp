#include "ActorFactory.h"
#include "ActorLUT.h"
#include "S2_Actor.h"
#include "S2_Sprite.h"
#include "S2_Symbol.h"
#include "SymType.h"

#include "TextboxActor.h"
#include "ComplexActor.h"
#include "AnimActor.h"
#include "AnchorActor.h"

#include <assert.h>

namespace s2
{

SINGLETON_DEFINITION(ActorFactory);

ActorFactory::ActorFactory()
{
}

Actor* ActorFactory::Create(const Actor* parent, const Sprite* child) const
{
	assert(child);

// 	Actor* actor = ActorLUT::Instance()->Query(path);
// 	if (actor) {
// 		return actor;
// 	}

	Actor* actor = NULL;
	switch (child->GetSymbol()->Type())
	{
	case SYM_ANCHOR:
		actor = new AnchorActor(child, parent);
		break;
	case SYM_TEXTBOX:
		actor = new TextboxActor(child, parent);
		break;
	case SYM_COMPLEX:
		actor = new ComplexActor(child, parent);
		break;
	case SYM_ANIMATION:
		actor = new AnimActor(child, parent);
		break;
	default:
		actor = new Actor(child, parent);
	}

//	ActorLUT::Instance()->Insert(actor);

	return actor;
}

}