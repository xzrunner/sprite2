#include "DummySprite.h"
#include "SprVisitorParams.h"

namespace s2
{

bool DummySprite::TraverseChildren(SprVisitor& visitor, const SprVisitorParams& params) const
{
	if (IsHasProxy()) 
	{
		SprTreePath path = params.path;
		path.Pop();
		const Sprite* proxy = GetProxy(path);
		if (proxy) {
			return proxy->TraverseChildren(visitor, params);
		}
	}
	return true;
}

}