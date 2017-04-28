#include "ProxyHelper.h"
#include "ProxySymbol.h"
#include "ProxySprite.h"

#include <assert.h>

namespace s2
{

Sprite* ProxyHelper::BuildGroup(const std::vector<std::pair<const Actor*, Sprite*> >& items)
{
	Sprite* ret = NULL;
	if (items.size() == 1) 
	{
		ret = items[0].second;
	} 
	else if (items.size() > 1) 
	{
		ProxySymbol* sym = new ProxySymbol(items);
		ProxySprite* spr = new ProxySprite(sym);
		sym->RemoveReference();
		assert(sym->GetRefCount() == 1 && spr->GetRefCount() == 1);
		ret = spr;
	}
	return ret;
}

}