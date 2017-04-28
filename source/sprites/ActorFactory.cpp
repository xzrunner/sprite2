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

SINGLETON_DEFINITION(ActorFactory);

ActorFactory::ActorFactory()
{
}

Actor* ActorFactory::Create(const Actor* parent, const Sprite* child) const
{
	assert(child);

	Actor* actor = const_cast<Actor*>(child->QueryActor(parent));
	if (actor) {
		return actor;
	}

	switch (child->GetSymbol()->Type())
	{
	case SYM_ANCHOR:
		actor = new AnchorActor(child, parent);
		break;
	case SYM_SCALE9:
		actor = new Scale9Actor(child, parent);
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
	case SYM_PARTICLE3D:
		actor = new Particle3dActor(child, parent);
		break;
	case SYM_MASK:
		actor = new MaskActor(child, parent);
		break;
	case SYM_PROXY:
		actor = new ProxyActor(child, parent);
		break;
	default:
		actor = new Actor(child, parent);
	}

	return actor;
}

}