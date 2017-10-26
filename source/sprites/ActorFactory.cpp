#include "sprite2/ActorFactory.h"
#include "sprite2/Actor.h"
#include "sprite2/Sprite.h"
#include "sprite2/Symbol.h"
#include "sprite2/SymType.h"

#include "sprite2/AnchorActor.h"
#include "sprite2/Scale9Actor.h"
#include "sprite2/TextboxActor.h"
#include "sprite2/ComplexActor.h"
#include "sprite2/AnimActor.h"
#include "sprite2/Particle3dActor.h"
#include "sprite2/MaskActor.h"
#include "sprite2/ProxyActor.h"

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
	}

	return actor;
}

}