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

ActorPtr ActorFactory::Create(const ActorConstPtr& parent,
	                                        const SprConstPtr& child)
{
	assert(child);

	auto actor = child->QueryActor(parent);
	if (actor) {
		return actor;
	}

	switch (child->GetSymbol()->Type())
	{
	case SYM_ANCHOR:
		actor = std::make_shared<AnchorActor>(child, parent);
		break;
	case SYM_SCALE9:
		actor = std::make_shared<Scale9Actor>(child, parent);
		break;
	case SYM_TEXTBOX:
		actor = std::make_shared<TextboxActor>(child, parent);
		break;
	case SYM_COMPLEX:
		actor = std::make_shared<ComplexActor>(child, parent);
		break;
	case SYM_ANIMATION:
		actor = std::make_shared<AnimActor>(child, parent);
		break;
	case SYM_PARTICLE3D:
		actor = std::make_shared<Particle3dActor>(child, parent);
		break;
	case SYM_MASK:
		actor = std::make_shared<MaskActor>(child, parent);
		break;
	case SYM_PROXY:
		actor = std::make_shared<ProxyActor>(child, parent);
		break;
	default:
		actor = std::make_shared<Actor>(child, parent);
	}

	if (actor) {
		child->AddActor(actor);
		actor->Init();
	}

	return actor;
}

}