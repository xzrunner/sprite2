#include "SkeletonSprite.h"
#include "SkeletonSymbol.h"

namespace s2
{

SkeletonSprite::SkeletonSprite() 
{
}

SkeletonSprite::SkeletonSprite(Symbol* sym) 
	: Sprite(sym)
	, m_pose(VI_DOWNCASTING<SkeletonSymbol*>(sym)->GetSkeleton())
{
}

SkeletonSprite* SkeletonSprite::Clone() const
{
	return new SkeletonSprite(*this);
}

}