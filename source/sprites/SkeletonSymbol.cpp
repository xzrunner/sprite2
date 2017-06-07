#include "SkeletonSymbol.h"
#include "SymType.h"
#include "SkeletonSprite.h"
#include "SkeletonPose.h"
#include "Skeleton.h"
#include "RenderParams.h"
#include "S2_Sprite.h"
#include "DrawNode.h"
#ifndef S2_DISABLE_STATISTICS
#include "sprite2/StatSymbol.h"
#endif // S2_DISABLE_STATISTICS

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

RenderReturn SkeletonSymbol::Draw(const RenderParams& rp, const Sprite* spr) const
{
#ifndef S2_DISABLE_STATISTICS
	StatSymbol::Instance()->AddDrawCount(StatSymbol::SYM_TRAIL);
#endif // S2_DISABLE_STATISTICS

	if (!m_skeleton) {
		return RENDER_NO_DATA;
	}

	RenderParams* rp_child = RenderParamsPool::Instance()->Pop();
	*rp_child = rp;
	if (!DrawNode::Prepare(rp, spr, *rp_child)) {
		RenderParamsPool::Instance()->Push(rp_child); 
		return RENDER_INVISIBLE;
	}

	if (spr) {
		const SkeletonSprite* sk_spr = VI_DOWNCASTING<const SkeletonSprite*>(spr);
		sk_spr->GetPose().StoreToSkeleton(m_skeleton);
	}
	RenderReturn ret = m_skeleton->Draw(*rp_child);

	RenderParamsPool::Instance()->Push(rp_child); 

	return ret;
}

void SkeletonSymbol::SetSkeleton(Skeleton* skeleton)
{
	cu::RefCountObjAssign(m_skeleton, skeleton);
}

sm::rect SkeletonSymbol::GetBoundingImpl(const Sprite* spr, const Actor* actor, bool cache) const
{
	if (!m_skeleton) {
		return sm::rect(); // empty
	}
	if (spr) {
		const SkeletonSprite* sk_spr = VI_DOWNCASTING<const SkeletonSprite*>(spr);
		sk_spr->GetPose().StoreToSkeleton(m_skeleton);
	}
	return m_skeleton->GetBounding();
}

}