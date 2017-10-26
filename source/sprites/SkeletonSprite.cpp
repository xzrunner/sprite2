#include "sprite2/SkeletonSprite.h"
#include "sprite2/SkeletonSymbol.h"
#include "sprite2/Skeleton.h"
#ifndef S2_DISABLE_STATISTICS
#include "sprite2/StatSprCount.h"
#endif // S2_DISABLE_STATISTICS

namespace s2
{

SkeletonSprite::SkeletonSprite() 
{
#ifndef S2_DISABLE_STATISTICS
	StatSprCount::Instance()->Add(STAT_SYM_SKELETON);
#endif // S2_DISABLE_STATISTICS
}

SkeletonSprite::SkeletonSprite(const SkeletonSprite& spr)
	: Sprite(spr)
{
#ifndef S2_DISABLE_STATISTICS
	StatSprCount::Instance()->Add(STAT_SYM_SKELETON);
#endif // S2_DISABLE_STATISTICS
}

SkeletonSprite& SkeletonSprite::operator = (const SkeletonSprite& spr)
{
	Sprite::operator = (spr);
	return *this;
}

SkeletonSprite::SkeletonSprite(const SymPtr& sym, uint32_t id)
	: Sprite(sym, id)
	, m_pose(*S2_VI_PTR_DOWN_CAST<SkeletonSymbol>(GetSymbol())->GetSkeleton())
{
#ifndef S2_DISABLE_STATISTICS
	StatSprCount::Instance()->Add(STAT_SYM_SKELETON);
#endif // S2_DISABLE_STATISTICS
}

SkeletonSprite::~SkeletonSprite()
{
#ifndef S2_DISABLE_STATISTICS
	StatSprCount::Instance()->Subtract(STAT_SYM_SKELETON);
#endif // S2_DISABLE_STATISTICS	
}

}