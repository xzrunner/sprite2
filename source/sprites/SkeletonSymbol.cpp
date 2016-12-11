#include "SkeletonSymbol.h"
#include "SymType.h"
#include "SkeletonSprite.h"
#include "SkeletonPose.h"
#include "Skeleton.h"
#include "RenderParams.h"
#include "S2_Sprite.h"
#include "SprTreePath.h"

namespace s2
{

SkeletonSymbol::SkeletonSymbol()
	: m_skeleton(NULL)
{
}

SkeletonSymbol::SkeletonSymbol(uint32_t id)
	: Symbol(id)
	, m_skeleton(NULL)
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
		p.mt = spr->GetLocalMat() * params.mt;
		p.color = spr->GetColor() * params.color;
		if (p.path) {
			p.path->Push(spr->GetID());
		}

		const SkeletonSprite* sk_spr = VI_DOWNCASTING<const SkeletonSprite*>(spr);
		sk_spr->GetPose().StoreToSkeleton(m_skeleton);
	}
	m_skeleton->Draw(p);
	if (spr && p.path) {
		p.path->Pop();
	}
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