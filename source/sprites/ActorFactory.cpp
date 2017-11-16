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
		actor = CU_MAKE_SHARED<AnchorActor>(child, parent);
		break;
	case SYM_SCALE9:
		actor = CU_MAKE_SHARED<Scale9Actor>(child, parent);
		break;
	case SYM_TEXTBOX:
		actor = CU_MAKE_SHARED<TextboxActor>(child, parent);
		break;
	case SYM_COMPLEX:
		actor = CU_MAKE_SHARED<ComplexActor>(child, parent);
		break;
	case SYM_ANIMATION:
		actor = CU_MAKE_SHARED<AnimActor>(child, parent);
		break;
	case SYM_PARTICLE3D:
		actor = CU_MAKE_SHARED<Particle3dActor>(child, parent);
		break;
	case SYM_MASK:
		actor = CU_MAKE_SHARED<MaskActor>(child, parent);
		break;
	case SYM_PROXY:
		actor = CU_MAKE_SHARED<ProxyActor>(child, parent);
		break;
	default:
		actor = CU_MAKE_SHARED<Actor>(child, parent);
	}

	if (actor) {
		child->AddActor(actor);
	}

	return actor;
}

}