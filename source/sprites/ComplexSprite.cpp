#include "ComplexSprite.h"
#include "ComplexSymbol.h"
#include "ComplexActor.h"
#include "UpdateParams.h"
#include "SpriteVisitor.h"
#include "SprVisitorParams.h"
#include "SymType.h"
#include "ProxyHelper.h"
#ifndef S2_DISABLE_STATISTICS
#include "sprite2/StatSprCount.h"
#endif // S2_DISABLE_STATISTICS

#include "s2_typedef.h"

#include <assert.h>

namespace s2
{

ComplexSprite::ComplexSprite() 
	: m_action(-1)
{
#ifndef S2_DISABLE_STATISTICS
	StatSprCount::Instance()->Add(STAT_SYM_COMPLEX);
#endif // S2_DISABLE_STATISTICS
}

ComplexSprite::ComplexSprite(const ComplexSprite& spr)
	: Sprite(spr)
{
#ifndef S2_DISABLE_STATISTICS
	StatSprCount::Instance()->Add(STAT_SYM_COMPLEX);
#endif // S2_DISABLE_STATISTICS
}

ComplexSprite& ComplexSprite::operator = (const ComplexSprite& spr)
{
	Sprite::operator = (spr);
	return *this;
}

ComplexSprite::ComplexSprite(const SymPtr& sym, uint32_t id)
	: Sprite(sym, id)
	, m_action(-1)
{
#ifndef S2_DISABLE_STATISTICS
	StatSprCount::Instance()->Add(STAT_SYM_COMPLEX);
#endif // S2_DISABLE_STATISTICS
}

ComplexSprite::~ComplexSprite()
{
#ifndef S2_DISABLE_STATISTICS
	StatSprCount::Instance()->Subtract(STAT_SYM_COMPLEX);
#endif // S2_DISABLE_STATISTICS
}

void ComplexSprite::OnMessage(const UpdateParams& up, Message msg)
{
	// update inherit
	if (!up.IsForce() && !IsInheritUpdate()) {
		return;
	}

	UpdateParams* up_child = UpdateParamsPool::Instance()->Pop();
	*up_child = up;

	up_child->Push(shared_from_this());
	auto& children = S2_VI_PTR_DOWN_CAST<ComplexSymbol>(m_sym)->
		GetActionChildren(GetAction(up.GetActor()));
	for (auto& child : children) {
		up_child->SetActor(child->QueryActor(up.GetActor()));
		child->OnMessage(*up_child, msg);
	}

	UpdateParamsPool::Instance()->Push(up_child); 
}

bool ComplexSprite::Update(const UpdateParams& up)
{
	// update inherit
	if (!up.IsForce() && !IsInheritUpdate()) {
		return false;
	}

	// visible
	auto& actor = up.GetActor();
	bool visible = actor ? actor->IsVisible() : IsVisible();
	if (!visible) {
		return false;
	}

	bool dirty = false;

	UpdateParams* up_child = UpdateParamsPool::Instance()->Pop();
	*up_child = up;

	up_child->Push(shared_from_this());
	auto& children = S2_VI_PTR_DOWN_CAST<ComplexSymbol>(m_sym)->
		GetActionChildren(GetAction(actor));
	for (auto& child : children) 
	{
		up_child->SetActor(child->QueryActor(actor));
		if (child->Update(*up_child)) {
			dirty = true;
		}
	}

	UpdateParamsPool::Instance()->Push(up_child); 

	return dirty;
}

SprPtr ComplexSprite::FetchChildByName(int name, const ActorConstPtr& actor) const
{
	std::vector<std::pair<const ActorConstPtr, SprPtr>> group;
	auto& children = S2_VI_PTR_DOWN_CAST<ComplexSymbol>(m_sym)->
		GetAllChildren();
	for (auto& child : children) {
		if (child->GetName() == name) {
			group.push_back(std::make_pair(actor, child));
		}
	}
	return ProxyHelper::BuildGroup(group);
}

SprPtr ComplexSprite::FetchChildByIdx(int idx, const ActorPtr& actor) const
{
	auto& children = S2_VI_PTR_DOWN_CAST<ComplexSymbol>(m_sym)->
		GetAllChildren();
	if (idx >= 0 && idx < static_cast<int>(children.size())) {
		return children[idx];
	} else {
		return nullptr;
	}
}

VisitResult ComplexSprite::TraverseChildren(SpriteVisitor& visitor, const SprVisitorParams& params) const
{
	VisitResult ret = VISIT_OVER;
	auto& children = S2_VI_PTR_DOWN_CAST<ComplexSymbol>(m_sym)->
		GetActionChildren(GetAction(params.actor));
	SprVisitorParams cp = params;
	if (visitor.GetOrder()) {
		for (int i = 0, n = children.size(); i < n; ++i) 
		{
			auto& child = children[i];
			cp.actor = child->QueryActor(params.actor);
			if (!SpriteVisitor::VisitChild(visitor, cp, child, ret)) {
				break;
			}
		}
	} else {
		for (int i = children.size() - 1; i >= 0; --i) 
		{
			auto& child = children[i];
			cp.actor = child->QueryActor(params.actor);
			if (!SpriteVisitor::VisitChild(visitor, cp, child, ret)) {
				break;
			}
		}
	}
	return ret;
}

void ComplexSprite::SetAction(int action) 
{ 
	m_action = action; 
	SetBoundingDirty(true);
}

int ComplexSprite::GetAction(const ActorConstPtr& actor) const
{
	if (!actor) {
		return -1;
	}
	assert(actor->GetSpr()->GetSymbol()->Type() == SYM_COMPLEX);
	auto& comp_actor = std::static_pointer_cast<const ComplexActor>(actor);
	return comp_actor->GetAction();
}

}