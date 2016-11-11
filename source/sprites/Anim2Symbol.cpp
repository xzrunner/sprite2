#include "Anim2Symbol.h"
#include "SymType.h"
#include "RenderParams.h"
#include "S2_Sprite.h"

#include <rigging/rg_skeleton.h>

namespace s2
{

Anim2Symbol::Anim2Symbol()
	: m_skeleton(NULL)
{
}

Anim2Symbol::~Anim2Symbol()
{
	// todo release sk
}
	
int Anim2Symbol::Type() const
{
	return SYM_ANIM2;
}

void Anim2Symbol::Draw(const RenderParams& params, const Sprite* spr) const
{
	if (!m_skeleton) {
		return;
	}

	RenderParams p = params;
	if (spr) {
		p.mt = spr->GetTransMatrix() * params.mt;
		p.color = spr->Color() * params.color;
	}

	rg_skeleton_draw(m_skeleton, &p);
}

sm::rect Anim2Symbol::GetBounding(const Sprite* spr) const
{
// 	if (!m_skeleton) {
// 		return sm::rect();
// 	}
// 	if (spr) {
// 		const SkeletonSprite* sk_spr = VI_DOWNCASTING<const SkeletonSprite*>(spr);
// 		sk_spr->GetPose().StoreToSkeleton(m_skeleton);
// 	}
//	return m_skeleton->GetBounding();

	//////////////////////////////////////////////////////////////////////////

	return sm::rect(200, 200);
}

}