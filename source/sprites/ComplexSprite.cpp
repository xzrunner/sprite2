#include "ComplexSprite.h"
#include "ComplexSymbol.h"
#include "ComplexActor.h"
#include "UpdateParams.h"
#include "SpriteVisitor.h"
#include "SprVisitorParams.h"
#include "SymType.h"
#include "ProxyHelper.h"

#include <assert.h>

namespace s2
{

ComplexSprite::ComplexSprite() 
	: m_action(-1)
{
}

ComplexSprite::ComplexSprite(Symbol* sym, uint32_t id)
	: Sprite(sym, id)
	, m_action(-1)
{
}

ComplexSprite* ComplexSprite::Clone() const
{
	return new ComplexSprite(*this);
}

void ComplexSprite::OnMessage(const UpdateParams& up, Message msg)
{
	UpdateParams* up_child = UpdateParamsPool::Instance()->Pop();
	*up_child = up;

	up_child->Push(this);
	const std::vector<Sprite*>& children 
		= VI_DOWNCASTING<ComplexSymbol*>(m_sym)->GetActionChildren(GetAction(up.GetActor()));
	for (int i = 0, n = children.size(); i < n; ++i) {
		Sprite* child = children[i];
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
	const Actor* actor = up.GetActor();
	bool visible = actor ? actor->IsVisible() : IsVisible();
	if (!visible) {
		return false;
	}

	bool dirty = false;

	UpdateParams* up_child = UpdateParamsPool::Instance()->Pop();
	*up_child = up;

	up_child->Push(this);
	const std::vector<Sprite*>& children 
		= VI_DOWNCASTING<ComplexSymbol*>(m_sym)->GetActionChildren(GetAction(actor));
	for (int i = 0, n = children.size(); i < n; ++i) 
	{
		const Sprite* child = children[i];
		up_child->SetActor(child->QueryActor(actor));
		if (const_cast<Sprite*>(child)->Update(*up_child)) {
			dirty = true;
		}
	}

	UpdateParamsPool::Instance()->Push(up_child); 

	return dirty;
}

Sprite* ComplexSprite::FetchChildByName(int name, const Actor* actor) const
{
	std::vector<std::pair<const Actor*, Sprite*> > group;
	const std::vector<Sprite*>& children 
		= VI_DOWNCASTING<ComplexSymbol*>(m_sym)->GetAllChildren();
	for (int i = 0, n = children.size(); i < n; ++i) {
		Sprite* child = children[i];
		if (child->GetName() == name) {
			group.push_back(std::make_pair(actor, child));
		}
	}
	return ProxyHelper::BuildGroup(group);
}

Sprite* ComplexSprite::FetchChildByIdx(int idx, const Actor* actor) const
{
	Sprite* ret = NULL;
	const std::vector<Sprite*>& children 
		= VI_DOWNCASTING<ComplexSymbol*>(m_sym)->GetAllChildren();
	if (idx >= 0 && idx < children.size()) {
		ret = children[idx];
	}
	if (ret) {
		ret->AddReference();
	}
	return ret;
}

VisitResult ComplexSprite::TraverseChildren(SpriteVisitor& visitor, const SprVisitorParams& params) const
{
	VisitResult ret = VISIT_OVER;
	const std::vector<Sprite*>& children 
		= VI_DOWNCASTING<ComplexSymbol*>(m_sym)->GetActionChildren(GetAction(params.actor));
	SprVisitorParams cp = params;
	if (visitor.GetOrder()) {
		for (int i = 0, n = children.size(); i < n; ++i) 
		{
			Sprite* child = children[i];
			cp.actor = child->QueryActor(params.actor);
			if (!SpriteVisitor::VisitChild(visitor, cp, child, ret)) {
				break;
			}
		}
	} else {
		for (int i = children.size() - 1; i >= 0; --i) 
		{
			Sprite* child = children[i];
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

int ComplexSprite::GetAction(const Actor* actor) const
{
	if (!actor) {
		return -1;
	}
	assert(actor->GetSpr()->GetSymbol()->Type() == SYM_COMPLEX);
	const ComplexActor* comp_actor = static_cast<const ComplexActor*>(actor);
	return comp_actor->GetAction();
}

}