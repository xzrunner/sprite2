#include "MaskActor.h"
#include "S2_Sprite.h"
#include "S2_Symbol.h"

namespace s2
{

MaskActor::MaskActor(const Sprite* spr, const Actor* parent)
	: Actor(spr, parent) 
{
	sm::rect mask_aabb = spr->GetSymbol()->GetBounding(spr);
	GetAABB().SetStaticRect(mask_aabb);
}

}