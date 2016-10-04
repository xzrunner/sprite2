#include "SkeletonSymbol.h"
#include "SymType.h"
#include "SkeletonSprite.h"
#include "SkeletonPose.h"
#include "Skeleton.h"
#include "RenderParams.h"
#include "S2_Sprite.h"

namespace s2
{

SkeletonSymbol::SkeletonSymbol()
	: m_skeleton(NULL)
{
}

SkeletonSymbol::~SkeletonSymbol()
{
	if (m_skeleton) {
		m_skeleton->RemoveReference();
	}
}

int SkeletonSymbol::Type() const 
{ 
	return SYM_SKELETON; 
}

void SkeletonSymbol::Draw(const RenderParams& params, const Sprite* spr) const
{
	if (!m_skeleton) {
		return;
	}

	RenderParams p = params;
	if (spr) {
		p.mt = spr->GetTransMatrix() * params.mt;
		p.color = spr->Color() * params.color;

		const SkeletonSprite* sk_spr = VI_DOWNCASTING<const SkeletonSprite*>(spr);
		sk_spr->GetPose().StoreToSkeleton(m_skeleton);
	}
	m_skeleton->Draw(p);
}

sm::rect SkeletonSymbol::GetBounding(const Sprite* spr) const
{
	if (!m_skeleton) {
		return sm::rect();
	}
	if (spr) {
		const SkeletonSprite* sk_spr = VI_DOWNCASTING<const SkeletonSprite*>(spr);
		sk_spr->GetPose().StoreToSkeleton(m_skeleton);
	}
	return m_skeleton->GetBounding();
}

void SkeletonSymbol::SetSkeleton(Skeleton* skeleton)
{
	cu::RefCountObjAssign((Skeleton*)m_skeleton, skeleton);
}

}