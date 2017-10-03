#include "ProxyActor.h"
#include "S2_Sprite.h"
#include "S2_Symbol.h"
#include "SymType.h"
#include "ProxySymbol.h"
#include "ActorFactory.h"

#include <assert.h>

namespace s2
{

ProxyActor::ProxyActor(const SprConstPtr& spr, const ActorConstPtr& parent)
	: Actor(spr, parent)
{
	assert(spr->GetSymbol()->Type() == SYM_PROXY);
	auto& items = S2_VI_PTR_DOWN_CAST<const ProxySymbol>(spr->GetSymbol())->GetItems();
	for (int i = 0, n = items.size(); i < n; ++i) {
		ActorFactory::Create(items[i].first, items[i].second);
	}
}

#ifndef S2_DISABLE_FLATTEN

void ProxyActor::SetFlatten(const std::shared_ptr<ft::FTList>& ft, int pos)
{
	assert(GetSpr()->GetSymbol()->Type() == SYM_PROXY);
	auto& items = S2_VI_PTR_DOWN_CAST<const ProxySymbol>(GetSpr()->GetSymbol())->GetItems();
	for (auto& item : items) {
		std::const_pointer_cast<Actor, const Actor>(item.first)->SetFlatten(ft, pos);
	}
}

void ProxyActor::SetFlattenDirty()
{
	assert(GetSpr()->GetSymbol()->Type() == SYM_PROXY);
	auto& items = S2_VI_PTR_DOWN_CAST<const ProxySymbol>(GetSpr()->GetSymbol())->GetItems();
	for (auto& item : items) {
		std::const_pointer_cast<Actor, const Actor>(item.first)->SetFlattenDirty();
	}
}

void ProxyActor::CreateFlatten()
{
	assert(GetSpr()->GetSymbol()->Type() == SYM_PROXY);
	auto& items = S2_VI_PTR_DOWN_CAST<const ProxySymbol>(GetSpr()->GetSymbol())->GetItems();
	for (auto& item : items) {
		std::const_pointer_cast<Actor, const Actor>(item.first)->CreateFlatten();
	}
}

void ProxyActor::BuildFlatten()
{
	assert(GetSpr()->GetSymbol()->Type() == SYM_PROXY);
	auto& items = S2_VI_PTR_DOWN_CAST<const ProxySymbol>(GetSpr()->GetSymbol())->GetItems();
	for (auto& item : items) {
		if (!item.first->HasFlatten()) {
			std::const_pointer_cast<Actor, const Actor>(item.first)->BuildFlatten();
		}
	}
}

bool ProxyActor::HasFlatten() const
{
	bool ret = true;
	assert(GetSpr()->GetSymbol()->Type() == SYM_PROXY);
	auto& items = S2_VI_PTR_DOWN_CAST<const ProxySymbol>(GetSpr()->GetSymbol())->GetItems();
	for (auto& item : items) {
		if (!(item.first)->HasFlatten()) {
			ret = false;
		}
	}
	return ret;
}

void ProxyActor::FlattenUpdate(bool force)
{
	assert(GetSpr()->GetSymbol()->Type() == SYM_PROXY);
	auto& items = S2_VI_PTR_DOWN_CAST<const ProxySymbol>(GetSpr()->GetSymbol())->GetItems();
	for (auto& item : items) {
		std::const_pointer_cast<Actor, const Actor>(item.first)->FlattenUpdate(force);
	}
}

void ProxyActor::FlattenDraw(const RenderParams& rp) const
{
	assert(GetSpr()->GetSymbol()->Type() == SYM_PROXY);
	auto& items = S2_VI_PTR_DOWN_CAST<const ProxySymbol>(GetSpr()->GetSymbol())->GetItems();
	for (auto& item : items) {
		item.first->FlattenDraw(rp);
	}
}

void ProxyActor::FlattenSetFrame(int frame)
{
	assert(GetSpr()->GetSymbol()->Type() == SYM_PROXY);
	auto& items = S2_VI_PTR_DOWN_CAST<const ProxySymbol>(GetSpr()->GetSymbol())->GetItems();
	for (auto& item : items) {
		std::const_pointer_cast<Actor, const Actor>(item.first)->FlattenSetFrame(frame);
	}
}

#endif // S2_DISABLE_FLATTEN

}