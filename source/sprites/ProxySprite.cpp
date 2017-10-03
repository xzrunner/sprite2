#include "ProxySprite.h"
#include "ProxySymbol.h"
#include "ProxyHelper.h"
#include "S2_Actor.h"
#include "S2_Sprite.h"
#include "S2_Symbol.h"
#include "SymType.h"
#include "UpdateParams.h"
#include "SprVisitorParams.h"

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
		UpdateParams* up_child = UpdateParamsPool::Instance()->Pop();
		*up_child = up;
		up_child->SetActor(item.second->QueryActor(item.first));
		item.second->OnMessage(*up_child, msg);
		UpdateParamsPool::Instance()->Push(up_child); 
	}
}

bool ProxySprite::Update(const UpdateParams& up)
{
	bool ret = false;
	auto& items = S2_VI_PTR_DOWN_CAST<ProxySymbol>(m_sym)->GetItems();
	for (auto& item : items) 
	{
		UpdateParams* up_child = UpdateParamsPool::Instance()->Pop();
		*up_child = up;
		up_child->SetActor(item.second->QueryActor(item.first));
		if (item.second->Update(*up_child)) {
			ret = true;
		}
		UpdateParamsPool::Instance()->Push(up_child); 
	}
	return ret;
}

SprPtr ProxySprite::FetchChildByName(int name, const ActorConstPtr& actor) const
{
	std::vector<std::pair<const ActorConstPtr, SprPtr>> group;
	auto& items = S2_VI_PTR_DOWN_CAST<ProxySymbol>(m_sym)->GetItems();
	if (actor->GetSpr()->GetSymbol()->Type() == SYM_PROXY) {
		for (auto& item : items) {
			auto real_actor = item.second->QueryActor(item.first);
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
	std::vector<std::pair<const ActorConstPtr, SprPtr>> group;
	auto& items = S2_VI_PTR_DOWN_CAST<ProxySymbol>(m_sym)->GetItems();
	if (actor->GetSpr()->GetSymbol()->Type() == SYM_PROXY) {
		for (auto& item : items) {
			auto real_actor = item.second->QueryActor(item.first);
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
		auto real_actor = item.second->QueryActor(item.first);
		cp.actor = real_actor;
		VisitResult ret = item.second->TraverseChildren(visitor, cp);
		if (ret == VISIT_STOP) {
			return ret;
		}
	}
	return VISIT_OVER;
}

} 