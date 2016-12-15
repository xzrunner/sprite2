#include "ActorFactory.h"
#include "ActorLUT.h"
#include "S2_Actor.h"
#include "S2_Sprite.h"
#include "S2_Symbol.h"
#include "SymType.h"

#include "TextboxActor.h"
#include "DummyActor.h"

#include <assert.h>

namespace s2
{

SINGLETON_DEFINITION(ActorFactory);

ActorFactory::ActorFactory()
{
}

Actor* ActorFactory::Create(const SprTreePath& parent, const Sprite* child) const
{
	assert(child);

	SprTreePath path = parent;
	path.Push(child->GetID());

	Actor* actor = ActorLUT::Instance()->Query(path);
	if (actor) {
		return actor;
	}

	switch (child->GetSymbol()->Type())
	{
	case SYM_TEXTBOX:
		actor = new TextboxActor(child, path);
		break;
	default:
		actor = new DummyActor(child, path);
	}

	ActorLUT::Instance()->Insert(actor);

	return actor;
}

}