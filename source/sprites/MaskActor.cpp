#include "sprite2/MaskActor.h"
#include "sprite2/Sprite.h"
#include "sprite2/Symbol.h"

namespace s2
{

MaskActor::MaskActor(const SprConstPtr& spr, const ActorConstPtr& parent)
	: Actor(spr, parent) 
{
	sm::rect mask_aabb = spr->GetSymbol()->GetBounding(spr.get());
	GetAABB().SetStaticRect(mask_aabb);
}

}