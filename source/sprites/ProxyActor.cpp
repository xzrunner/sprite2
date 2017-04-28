#include "ProxyActor.h"
#include "S2_Sprite.h"
#include "S2_Symbol.h"
#include "SymType.h"
#include "ProxySymbol.h"
#include "ActorFactory.h"

#include <assert.h>

namespace s2
{

ProxyActor::ProxyActor(const Sprite* spr, const Actor* parent)
	: Actor(spr, parent) 
{
	assert(spr->GetSymbol()->Type() == SYM_PROXY);
	const std::vector<std::pair<const Actor*, Sprite*> >& items 
		= VI_DOWNCASTING<const ProxySymbol*>(spr->GetSymbol())->GetItems();
	for (int i = 0, n = items.size(); i < n; ++i) {
		ActorFactory::Instance()->Create(items[i].first, items[i].second);
	}
}

}