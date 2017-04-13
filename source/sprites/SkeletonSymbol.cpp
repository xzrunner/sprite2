#include "SkeletonSymbol.h"
#include "SymType.h"
#include "SkeletonSprite.h"
#include "SkeletonPose.h"
#include "Skeleton.h"
#include "RenderParams.h"
#include "S2_Sprite.h"
#include "DrawNode.h"

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

void SkeletonSymbol::Draw(const RenderParams& rp, const Sprite* spr) const
{
	if (!m_skeleton) {
		return;
	}

	RenderParams rp_child(rp);
	if (!DrawNode::Prepare(rp, spr, rp_child)) {
		return;
	}

	if (spr) {
		const SkeletonSprite* sk_spr = VI_DOWNCASTING<const SkeletonSprite*>(spr);
		sk_spr->GetPose().StoreToSkeleton(m_skeleton);
	}
	m_skeleton->Draw(rp_child);
}

sm::rect SkeletonSymbol::GetBounding(const Sprite* spr, const Actor* actor, bool cache) const
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
	cu::RefCountObjAssign(m_skeleton, skeleton);
}

}