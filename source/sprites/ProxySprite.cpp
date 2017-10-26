#include "sprite2/ProxySprite.h"
#include "sprite2/ProxySymbol.h"
#include "sprite2/ProxyHelper.h"
#include "sprite2/Actor.h"
#include "sprite2/Sprite.h"
#include "sprite2/Symbol.h"
#include "sprite2/SymType.h"
#include "sprite2/UpdateParams.h"
#include "sprite2/SprVisitorParams.h"

namespace s2
{

ProxySprite::ProxySprite(const SymPtr& sym)
	: Sprite(sym)
{
}

void ProxySprite::OnMessage(const UpdateParams& up, Message msg)
{
	auto& items = S2_VI_PTR_DOWN_CAST<ProxySymbol>(m_sym)->GetItems();
	for (auto& item : items) 
	{
		UpdateParams up_child(up);
		up_child.SetActor(item.second->QueryActor(item.first.get()));
		item.second->OnMessage(up_child, msg);
	}
}

bool ProxySprite::Update(const UpdateParams& up)
{
	bool ret = false;
	auto& items = S2_VI_PTR_DOWN_CAST<ProxySymbol>(m_sym)->GetItems();
	for (auto& item : items) 
	{
		UpdateParams up_child(up);
		up_child.SetActor(item.second->QueryActor(item.first.get()));
		if (item.second->Update(up_child)) {
			ret = true;
		}
	}
	return ret;
}

SprPtr ProxySprite::FetchChildByName(int name, const ActorConstPtr& actor) const
{
	CU_VEC<std::pair<const ActorConstPtr, SprPtr>> group;
	auto& items = S2_VI_PTR_DOWN_CAST<ProxySymbol>(m_sym)->GetItems();
	if (actor->GetSprRaw()->GetSymbol()->Type() == SYM_PROXY) {
		for (auto& item : items) {
			auto real_actor = item.second->QueryActorRef(item.first.get());
			auto child = item.second->FetchChildByName(name, real_actor);
			if (child) {
				group.push_back(std::make_pair(real_actor, child));
			}
		}
	} else {
		for (auto& item : items) {
			SprPtr child = item.second->FetchChildByName(name, actor);
			if (child) {
				group.push_back(std::make_pair(actor, child));
			}
		}
	}
	return ProxyHelper::BuildGroup(group, true);
}

SprPtr ProxySprite::FetchChildByIdx(int idx, const ActorPtr& actor) const
{
	CU_VEC<std::pair<const ActorConstPtr, SprPtr>> group;
	auto& items = S2_VI_PTR_DOWN_CAST<ProxySymbol>(m_sym)->GetItems();
	if (actor->GetSprRaw()->GetSymbol()->Type() == SYM_PROXY) {
		for (auto& item : items) {
			auto real_actor = item.second->QueryActorRef(item.first.get());
			auto child = item.second->FetchChildByIdx(idx, real_actor);
			if (child) {
				group.push_back(std::make_pair(real_actor, child));
			}
		}
	} else {
		for (auto& item : items) {
			auto child = item.second->FetchChildByIdx(idx, actor);
			if (child) {
				group.push_back(std::make_pair(actor, child));
			}
		}
	}
	return ProxyHelper::BuildGroup(group, true);
}

void ProxySprite::SetSymbol(const SymPtr& sym)
{
	auto& items = S2_VI_PTR_DOWN_CAST<ProxySymbol>(m_sym)->GetItems();
	for (auto& item : items) {
		item.second->SetSymbol(sym);
	}
}

void ProxySprite::SetCenter(const sm::vec2& pos)
{
	auto& items = S2_VI_PTR_DOWN_CAST<ProxySymbol>(m_sym)->GetItems();
	for (auto& item : items) {
		item.second->SetCenter(pos);
	}
}

void ProxySprite::SetPosition(const sm::vec2& pos)
{
	auto& items = S2_VI_PTR_DOWN_CAST<ProxySymbol>(m_sym)->GetItems();
	for (auto& item : items) {
		item.second->SetPosition(pos);
	}
}

void ProxySprite::SetAngle(float angle)
{
	auto& items = S2_VI_PTR_DOWN_CAST<ProxySymbol>(m_sym)->GetItems();
	for (auto& item : items) {
		item.second->SetAngle(angle);
	}
}

void ProxySprite::SetScale(const sm::vec2& scale)
{
	auto& items = S2_VI_PTR_DOWN_CAST<ProxySymbol>(m_sym)->GetItems();
	for (auto& item : items) {
		item.second->SetScale(scale);
	}
}

void ProxySprite::SetShear(const sm::vec2& shear)
{
	auto& items = S2_VI_PTR_DOWN_CAST<ProxySymbol>(m_sym)->GetItems();
	for (auto& item : items) {
		item.second->SetShear(shear);
	}
}

void ProxySprite::SetOffset(const sm::vec2& offset)
{
	auto& items = S2_VI_PTR_DOWN_CAST<ProxySymbol>(m_sym)->GetItems();
	for (auto& item : items) {
		item.second->SetOffset(offset);
	}
}

VisitResult ProxySprite::TraverseChildren(SpriteVisitor& visitor, const SprVisitorParams& params) const
{
	auto& items = S2_VI_PTR_DOWN_CAST<ProxySymbol>(m_sym)->GetItems();
	SprVisitorParams cp = params;
	for (auto& item : items) 
	{
		cp.actor = item.second->QueryActor(item.first.get());
		VisitResult ret = item.second->TraverseChildren(visitor, cp);
		if (ret == VISIT_STOP) {
			return ret;
		}
	}
	return VISIT_OVER;
}

VisitResult ProxySprite::TraverseChildren2(SpriteVisitor2& visitor, const SprVisitorParams2& params) const
{
	auto& items = S2_VI_PTR_DOWN_CAST<ProxySymbol>(m_sym)->GetItems();
	SprVisitorParams2 cp = params;
	for (auto& item : items)
	{
		cp.actor = item.second->QueryActorRef(item.first.get());
		VisitResult ret = item.second->TraverseChildren2(visitor, cp);
		if (ret == VISIT_STOP) {
			return ret;
		}
	}
	return VISIT_OVER;
}

} 