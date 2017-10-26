#include "sprite2/CacheMatVisitor.h"
#include "sprite2/Sprite.h"

namespace s2
{

void CacheMatVisitor::Visit(const SprPtr& spr) const
{
#ifdef S2_SPR_CACHE_LOCAL_MAT_SHARE
	spr->CacheLocalMat();
#endif // S2_SPR_CACHE_LOCAL_MAT_SHARE
}

}