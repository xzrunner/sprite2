#include "ComplexSprite.h"
#include "ComplexSymbol.h"
#include "ComplexActor.h"
#include "RenderParams.h"
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

void ComplexSprite::OnMessage(Message msg, const Actor* actor)
{
	const std::vector<Sprite*>& children 
		= VI_DOWNCASTING<ComplexSymbol*>(m_sym)->GetActionChildren(m_action);
	for (int i = 0, n = children.size(); i < n; ++i) {
		Sprite* spr = children[i];
		spr->OnMessage(msg, spr->QueryActor(actor));
	}
}

bool ComplexSprite::Update(const RenderParams& rp)
{
	const Actor* actor = rp.actor;

	RenderParams rp_child(rp);
	rp_child.mt = GetLocalMat() * rp.mt;
	rp_child.shader = GetShader() * rp.shader;
	if (rp.actor) {
		rp_child.mt = rp.actor->GetLocalMat() * rp_child.mt;
		rp_child.shader = rp.actor->GetShader() * rp_child.shader;
	}

	bool dirty = false;
	int action = m_action;
	if (actor) {
		const ComplexActor* comp_actor = static_cast<const ComplexActor*>(actor);
		action = comp_actor->GetAction();
	}
	const std::vector<Sprite*>& children 
		= VI_DOWNCASTING<ComplexSymbol*>(m_sym)->GetActionChildren(action);
	for (int i = 0, n = children.size(); i < n; ++i) 
	{
		const Sprite* spr = children[i];
		if(spr->IsForceUpFrame() || spr->GetName().empty()) {
			rp_child.actor = spr->QueryActor(rp.actor);
			if (const_cast<Sprite*>(spr)->Update(rp_child)) {
				dirty = true;
			}
		}
	}
	return dirty;
}

bool ComplexSprite::SetFrame(int frame, const Actor* actor, bool force)
{
	if (!force && !IsForceUpFrame() && !GetName().empty()) {
		return false;
	}

	bool dirty = false;
	int action = m_action;
	if (actor) {
		const ComplexActor* comp_actor = static_cast<const ComplexActor*>(actor);
		action = comp_actor->GetAction();
	}
	const std::vector<Sprite*>& children 
		= VI_DOWNCASTING<ComplexSymbol*>(m_sym)->GetActionChildren(action);
	for (int i = 0, n = children.size(); i < n; ++i) {
		Sprite* child = children[i];
		if (child->SetFrame(frame, child->QueryActor(actor))) {
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

VisitResult ComplexSprite::TraverseChildren(SpriteVisitor& visitor, const SprVisitorParams& params) const
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
	return ret;
}


}