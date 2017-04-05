#include "ComplexSprite.h"
#include "ComplexSymbol.h"
#include "ComplexActor.h"
#include "UpdateParams.h"
#include "SpriteVisitor.h"
#include "SprVisitorParams.h"
#include "SymType.h"

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
	UpdateParams up_child(up);
	up_child.Push(this);
	const std::vector<Sprite*>& children 
		= VI_DOWNCASTING<ComplexSymbol*>(m_sym)->GetActionChildren(m_action);
	for (int i = 0, n = children.size(); i < n; ++i) {
		Sprite* child = children[i];
		up_child.SetActor(child->QueryActor(up.GetActor()));
		child->OnMessage(up_child, msg);
	}
}

bool ComplexSprite::Update(const UpdateParams& up)
{
	bool dirty = false;

	int action = m_action;
	const Actor* actor = up.GetActor();
	if (actor) {
		const ComplexActor* comp_actor = static_cast<const ComplexActor*>(actor);
		action = comp_actor->GetAction();
	}

	UpdateParams up_child(up);
	up_child.Push(this);
	const std::vector<Sprite*>& children 
		= VI_DOWNCASTING<ComplexSymbol*>(m_sym)->GetActionChildren(action);
	for (int i = 0, n = children.size(); i < n; ++i) 
	{
		const Sprite* child = children[i];
		if (child->ShouldInheritFrame()) {
			up_child.SetActor(child->QueryActor(up.GetActor()));
			if (const_cast<Sprite*>(child)->Update(up_child)) {
				dirty = true;
			}
		}
	}
	return dirty;
}

bool ComplexSprite::SetFrame(const UpdateParams& up, int frame, bool force)
{
	if (!force && !ShouldInheritFrame()) {
		return false;
	}

	bool dirty = false;
	int action = m_action;
	const Actor* actor = up.GetActor();
	if (actor) {
		const ComplexActor* comp_actor = static_cast<const ComplexActor*>(actor);
		action = comp_actor->GetAction();
	}
	UpdateParams up_child(up);
	up_child.Push(this);
	const std::vector<Sprite*>& children 
		= VI_DOWNCASTING<ComplexSymbol*>(m_sym)->GetActionChildren(action);
	for (int i = 0, n = children.size(); i < n; ++i) {
		Sprite* child = children[i];
		up_child.SetActor(child->QueryActor(up.GetActor()));
		if (child->SetFrame(up_child, frame)) {
			dirty = true;
		}
	}
	return dirty;
}

Sprite* ComplexSprite::FetchChild(const std::string& name, const Actor* actor) const
{
	const std::vector<Sprite*>& children 
		= VI_DOWNCASTING<ComplexSymbol*>(m_sym)->GetAllChildren();
	for (int i = 0, n = children.size(); i < n; ++i) {
		Sprite* child = children[i];
		if (child->GetName() == name) {
			return child;
		}
	}
	return NULL;
}

Sprite* ComplexSprite::FetchChild(int idx, const Actor* actor) const
{
	const std::vector<Sprite*>& children 
		= VI_DOWNCASTING<ComplexSymbol*>(m_sym)->GetAllChildren();
	if (idx >= 0 && idx < children.size()) {
		return children[idx];
	} else {
		return NULL;
	}
}

VisitResult ComplexSprite::TraverseChildren(SpriteVisitor& visitor, SprVisitorParams& params) const
{
	VisitResult ret = VISIT_OVER;
	int action = m_action;
	const Actor* actor = params.actor;
	if (actor) {
		const ComplexActor* comp_actor = static_cast<const ComplexActor*>(actor);
		action = comp_actor->GetAction();
	}
	const std::vector<Sprite*>& children 
		= VI_DOWNCASTING<ComplexSymbol*>(m_sym)->GetActionChildren(action);
	SprVisitorParams cp = params;
	if (visitor.GetOrder()) {
		for (int i = 0, n = children.size(); i < n; ++i) 
		{
			Sprite* child = children[i];
			cp.actor = child->QueryActor(actor);
			if (!SpriteVisitor::VisitChild(visitor, cp, child, ret)) {
				break;
			}
		}
	} else {
		for (int i = children.size() - 1; i >= 0; --i) 
		{
			Sprite* child = children[i];
			cp.actor = child->QueryActor(actor);
			if (!SpriteVisitor::VisitChild(visitor, cp, child, ret)) {
				break;
			}
		}
	}
	params.selected = cp.selected;
	return ret;
}

void ComplexSprite::SetAction(int action) 
{ 
	m_action = action; 
	SetBoundingDirty(true);
}

}