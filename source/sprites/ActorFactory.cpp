#include "ActorFactory.h"
#include "S2_Actor.h"
#include "S2_Sprite.h"
#include "S2_Symbol.h"
#include "SymType.h"

#include "AnchorActor.h"
#include "Scale9Actor.h"
#include "TextboxActor.h"
#include "ComplexActor.h"
#include "AnimActor.h"
#include "Particle3dActor.h"
#include "MaskActor.h"
#include "ProxyActor.h"

#include <assert.h>

namespace s2
{

ActorPtr ActorFactory::Create(const ActorConstPtr& parent, const SprConstPtr& child)
{
	assert(child);

	auto actor = child->QueryActorRef(parent.get());
	if (actor) {
		return actor;
	}

	switch (child->GetSymbol()->Type())
	{
	case SYM_ANCHOR:
		actor = mm::allocate_shared<AnchorActor>(child, parent);
		break;
	case SYM_SCALE9:
		actor = mm::allocate_shared<Scale9Actor>(child, parent);
		break;
	case SYM_TEXTBOX:
		actor = mm::allocate_shared<TextboxActor>(child, parent);
		break;
	case SYM_COMPLEX:
		actor = mm::allocate_shared<ComplexActor>(child, parent);
		break;
	case SYM_ANIMATION:
		actor = mm::allocate_shared<AnimActor>(child, parent);
		break;
	case SYM_PARTICLE3D:
		actor = mm::allocate_shared<Particle3dActor>(child, parent);
		break;
	case SYM_MASK:
		actor = mm::allocate_shared<MaskActor>(child, parent);
		break;
	case SYM_PROXY:
		actor = mm::allocate_shared<ProxyActor>(child, parent);
		break;
	default:
		actor = mm::allocate_shared<Actor>(child, parent);
	}

	if (actor) {
		child->AddActor(actor);
		actor->Init();
	}

	return actor;
}

}