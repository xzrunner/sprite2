#include "ProxyHelper.h"
#include "ProxySymbol.h"
#include "ProxySprite.h"

#include <assert.h>

namespace s2
{

Sprite* ProxyHelper::BuildGroup(const std::vector<Sprite*>& sprs, const Actor* actor)
{
	Sprite* ret = NULL;
	if (sprs.size() == 1) {
		ret = sprs[0];
	} else if (sprs.size() > 1) {
		ProxySymbol* sym = new ProxySymbol(sprs, actor);
		ProxySprite* spr = new ProxySprite(sym);
		sym->RemoveReference();
		assert(sym->GetRefCount() == 1 && spr->GetRefCount() == 1);
		ret = spr;
	}
	return ret;
}

}