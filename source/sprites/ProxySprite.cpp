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

ProxySprite::ProxySprite(Symbol* sym)
	: Sprite(sym)
{
}

ProxySprite* ProxySprite::Clone() const
{
	return new ProxySprite(*this);
}

void ProxySprite::OnMessage(const UpdateParams& up, Message msg)
{
	const std::vector<std::pair<const Actor*, Sprite*> >& items 
		= VI_DOWNCASTING<ProxySymbol*>(m_sym)->GetItems();
	for (int i = 0, n = items.size(); i < n; ++i) 
	{
		UpdateParams up_child(up);
		up_child.SetActor(items[i].second->QueryActor(items[i].first));
		items[i].second->OnMessage(up_child, msg);
	}
}

bool ProxySprite::Update(const UpdateParams& up)
{
	bool ret = false;
	const std::vector<std::pair<const Actor*, Sprite*> >& items 
		= VI_DOWNCASTING<ProxySymbol*>(m_sym)->GetItems();
	for (int i = 0, n = items.size(); i < n; ++i) 
	{
		UpdateParams up_child(up);
		up_child.SetActor(items[i].second->QueryActor(items[i].first));
		if (items[i].second->Update(up_child)) {
			ret = true;
		}
	}
	return ret;
}

Sprite* ProxySprite::FetchChildByName(int name, const Actor* actor) const
{
	std::vector<std::pair<const Actor*, Sprite*> > group;
	const std::vector<std::pair<const Actor*, Sprite*> >& items 
		= VI_DOWNCASTING<ProxySymbol*>(m_sym)->GetItems();
	if (actor->GetSpr()->GetSymbol()->Type() == SYM_PROXY) {
		for (int i = 0, n = items.size(); i < n; ++i) {
			const Actor* real_actor = items[i].second->QueryActor(items[i].first);
			Sprite* child = items[i].second->FetchChildByName(name, real_actor);
			if (child) {
				group.push_back(std::make_pair(real_actor, child));
			}
		}
	} else {
		for (int i = 0, n = items.size(); i < n; ++i) {
			Sprite* child = items[i].second->FetchChildByName(name, actor);
			if (child) {
				group.push_back(std::make_pair(actor, child));
			}
		}
	}
	return ProxyHelper::BuildGroup(group);
}

Sprite* ProxySprite::FetchChildByIdx(int idx, const Actor* actor) const
{
	std::vector<std::pair<const Actor*, Sprite*> > group;
	const std::vector<std::pair<const Actor*, Sprite*> >& items 
		= VI_DOWNCASTING<ProxySymbol*>(m_sym)->GetItems();
	if (actor->GetSpr()->GetSymbol()->Type() == SYM_PROXY) {
		for (int i = 0, n = items.size(); i < n; ++i) {
			const Actor* real_actor = items[i].second->QueryActor(items[i].first);
			Sprite* child = items[i].second->FetchChildByIdx(idx, real_actor);
			if (child) {
				group.push_back(std::make_pair(real_actor, child));
			}
		}
	} else {
		for (int i = 0, n = items.size(); i < n; ++i) {
			Sprite* child = items[i].second->FetchChildByIdx(idx, actor);
			if (child) {
				group.push_back(std::make_pair(actor, child));
			}
		}
	}
	return ProxyHelper::BuildGroup(group);
}

void ProxySprite::SetSymbol(Symbol* sym)
{
	const std::vector<std::pair<const Actor*, Sprite*> >& items 
		= VI_DOWNCASTING<ProxySymbol*>(m_sym)->GetItems();
	for (int i = 0, n = items.size(); i < n; ++i) {
		items[i].second->SetSymbol(sym);
	}
}

void ProxySprite::SetCenter(const sm::vec2& pos)
{
	const std::vector<std::pair<const Actor*, Sprite*> >& items 
		= VI_DOWNCASTING<ProxySymbol*>(m_sym)->GetItems();
	for (int i = 0, n = items.size(); i < n; ++i) {
		items[i].second->SetCenter(pos);
	}
}

void ProxySprite::SetPosition(const sm::vec2& pos)
{
	const std::vector<std::pair<const Actor*, Sprite*> >& items 
		= VI_DOWNCASTING<ProxySymbol*>(m_sym)->GetItems();
	for (int i = 0, n = items.size(); i < n; ++i) {
		items[i].second->SetPosition(pos);
	}
}

void ProxySprite::SetAngle(float angle)
{
	const std::vector<std::pair<const Actor*, Sprite*> >& items 
		= VI_DOWNCASTING<ProxySymbol*>(m_sym)->GetItems();
	for (int i = 0, n = items.size(); i < n; ++i) {
		items[i].second->SetAngle(angle);
	}
}

void ProxySprite::SetScale(const sm::vec2& scale)
{
	const std::vector<std::pair<const Actor*, Sprite*> >& items 
		= VI_DOWNCASTING<ProxySymbol*>(m_sym)->GetItems();
	for (int i = 0, n = items.size(); i < n; ++i) {
		items[i].second->SetScale(scale);
	}
}

void ProxySprite::SetShear(const sm::vec2& shear)
{
	const std::vector<std::pair<const Actor*, Sprite*> >& items 
		= VI_DOWNCASTING<ProxySymbol*>(m_sym)->GetItems();
	for (int i = 0, n = items.size(); i < n; ++i) {
		items[i].second->SetShear(shear);
	}
}

void ProxySprite::SetOffset(const sm::vec2& offset)
{
	const std::vector<std::pair<const Actor*, Sprite*> >& items 
		= VI_DOWNCASTING<ProxySymbol*>(m_sym)->GetItems();
	for (int i = 0, n = items.size(); i < n; ++i) {
		items[i].second->SetOffset(offset);
	}
}

VisitResult ProxySprite::TraverseChildren(SpriteVisitor& visitor, const SprVisitorParams& params) const
{
	const std::vector<std::pair<const Actor*, Sprite*> >& items 
		= VI_DOWNCASTING<ProxySymbol*>(m_sym)->GetItems();
	SprVisitorParams cp = params;
	for (int i = 0, n = items.size(); i < n; ++i) 
	{
		const Actor* real_actor = items[i].second->QueryActor(items[i].first);
		cp.actor = real_actor;
		VisitResult ret = items[i].second->TraverseChildren(visitor, cp);
		if (ret == VISIT_STOP) {
			return ret;
		}
	}
	return VISIT_OVER;
}

} 