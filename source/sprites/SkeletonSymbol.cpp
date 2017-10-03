#include "SkeletonSymbol.h"
#include "SymType.h"
#include "SkeletonSprite.h"
#include "SkeletonPose.h"
#include "RenderParams.h"
#include "S2_Sprite.h"
#include "DrawNode.h"
#ifndef S2_DISABLE_STATISTICS
#include "sprite2/StatSymDraw.h"
#include "sprite2/StatSymCount.h"
#endif // S2_DISABLE_STATISTICS

namespace s2
{

SkeletonSymbol::SkeletonSymbol()
	: m_skeleton(nullptr)
{
#ifndef S2_DISABLE_STATISTICS
	StatSymCount::Instance()->Add(STAT_SYM_SKELETON);
#endif // S2_DISABLE_STATISTICS
}

SkeletonSymbol::SkeletonSymbol(uint32_t id)
	: Symbol(id)
	, m_skeleton(nullptr)
{
#ifndef S2_DISABLE_STATISTICS
	StatSymCount::Instance()->Add(STAT_SYM_SKELETON);
#endif // S2_DISABLE_STATISTICS
}

SkeletonSymbol::~SkeletonSymbol()
{
#ifndef S2_DISABLE_STATISTICS
	StatSymCount::Instance()->Subtract(STAT_SYM_SKELETON);
#endif // S2_DISABLE_STATISTICS
}

int SkeletonSymbol::Type() const 
{ 
	return SYM_SKELETON; 
}

RenderReturn SkeletonSymbol::DrawTree(const RenderParams& rp, const SprConstPtr& spr) const
{
#ifndef S2_DISABLE_STATISTICS
	StatSymDraw::Instance()->AddDrawCount(STAT_SYM_SKELETON);
//	StatSymDraw::DrawCostCP cp(STAT_SYM_SKELETON);
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
		auto& sk_spr = S2_VI_PTR_DOWN_CAST<const SkeletonSprite>(spr);
		if (m_skeleton) {
			sk_spr->GetPose().StoreToSkeleton(*m_skeleton);
		}
	}
	RenderReturn ret = m_skeleton->Draw(*rp_child);

	RenderParamsPool::Instance()->Push(rp_child); 

	return ret;
}

sm::rect SkeletonSymbol::GetBoundingImpl(const SprConstPtr& spr, const ActorConstPtr& actor, bool cache) const
{
	if (!m_skeleton) {
		return sm::rect(); // empty
	}
	if (spr) {
		auto& sk_spr = S2_VI_PTR_DOWN_CAST<const SkeletonSprite>(spr);
		if (m_skeleton) {
			sk_spr->GetPose().StoreToSkeleton(*m_skeleton);
		}
	}
	return m_skeleton->GetBounding();
}

}