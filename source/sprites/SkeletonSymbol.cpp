#include "sprite2/SkeletonSymbol.h"
#include "sprite2/SymType.h"
#include "sprite2/SkeletonSprite.h"
#include "sprite2/SkeletonPose.h"
#include "sprite2/RenderParams.h"
#include "sprite2/Sprite.h"
#include "sprite2/DrawNode.h"
#ifndef S2_DISABLE_STATISTICS
#include "sprite2/StatSymDraw.h"
#include "sprite2/StatSymCount.h"
#endif // S2_DISABLE_STATISTICS

#include <memmgr/Allocator.h>
#include <cooking/DisplayList.h>

namespace s2
{

SkeletonSymbol::SkeletonSymbol()
{
#ifndef S2_DISABLE_STATISTICS
	StatSymCount::Instance()->Add(STAT_SYM_SKELETON);
#endif // S2_DISABLE_STATISTICS
}

SkeletonSymbol::SkeletonSymbol(uint32_t id)
	: Symbol(id)
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

RenderReturn SkeletonSymbol::DrawTree(cooking::DisplayList* dlist, const RenderParams& rp, const Sprite* spr) const
{
#ifndef S2_DISABLE_STATISTICS
	StatSymDraw::Instance()->AddDrawCount(STAT_SYM_SKELETON);
//	StatSymDraw::DrawCostCP cp(STAT_SYM_SKELETON);
#endif // S2_DISABLE_STATISTICS

	if (!m_skeleton) {
		return RENDER_NO_DATA;
	}

#ifndef S2_DISABLE_DEFERRED
	RenderParamsProxy rp_proxy(dlist->GetThreadIdx());
#else
	RenderParamsProxy rp_proxy;
#endif // S2_DISABLE_DEFERRED
	RenderParams* rp_child = rp_proxy.obj;
	memcpy(rp_child, &rp, sizeof(rp));

	if (!DrawNode::Prepare(rp, spr, *rp_child)) {
		return RENDER_INVISIBLE;
	}

	if (spr) {
		auto sk_spr = S2_VI_DOWN_CAST<const SkeletonSprite*>(spr);
		if (m_skeleton) {
			sk_spr->GetPose().StoreToSkeleton(*m_skeleton);
		}
	}
	return m_skeleton->Draw(dlist, *rp_child);
}

sm::rect SkeletonSymbol::GetBoundingImpl(const Sprite* spr, const Actor* actor, bool cache) const
{
	if (!m_skeleton) {
		return sm::rect(); // empty
	}
	if (spr) {
		auto sk_spr = S2_VI_DOWN_CAST<const SkeletonSprite*>(spr);
		if (m_skeleton) {
			sk_spr->GetPose().StoreToSkeleton(*m_skeleton);
		}
	}
	return m_skeleton->GetBounding();
}

}