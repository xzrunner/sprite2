#include "MaskActor.h"
#include "S2_Sprite.h"
#include "S2_Symbol.h"

namespace s2
{

MaskActor::MaskActor(const SprConstPtr& spr, const ActorConstPtr& parent)
	: Actor(spr, parent) 
{
	sm::rect mask_aabb = spr->GetSymbol()->GetBounding(spr.get());
	GetAABB().SetStaticRect(mask_aabb);
}

}