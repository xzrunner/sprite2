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

void ComplexSprite::OnMessage(Message msg, const SprTreePath& path)
{
	SprTreePath cpath = path;
	cpath.Push(*this);
	const std::vector<Sprite*>& children 
		= VI_DOWNCASTING<ComplexSymbol*>(m_sym)->GetActionChildren(m_action);
	for (int i = 0, n = children.size(); i < n; ++i) {
		children[i]->OnMessage(msg, cpath);
	}
}

bool ComplexSprite::Update(const RenderParams& rp)
{
	RenderParams rp_child = rp;
	rp_child.mt = GetLocalMat() * rp.mt;
	rp_child.shader = GetShader() * rp.shader;
	rp_child.path.Push(*this);

	bool dirty = false;
	int action = m_action;
	const Actor* actor = QueryActor(rp_child.path);
	if (actor) {
		const ComplexActor* comp_actor = static_cast<const ComplexActor*>(actor);
		action = comp_actor->GetAction();
	}
	const std::vector<Sprite*>& children 
		= VI_DOWNCASTING<ComplexSymbol*>(m_sym)->GetActionChildren(action);
	for (int i = 0, n = children.size(); i < n; ++i) {
		const Sprite* spr = children[i];
		if (const_cast<Sprite*>(spr)->Update(rp_child)) {
			dirty = true;
		}
	}
	return dirty;
}

bool ComplexSprite::SetFrame(int frame, const SprTreePath& path, bool force)
{
	if (!force && !IsForceUpFrame() && !GetName().empty()) {
		return false;
	}

	bool dirty = false;
	int action = m_action;
	SprTreePath cpath = path;
	cpath.Push(*this);
	const Actor* actor = QueryActor(cpath);
	if (actor) {
		const ComplexActor* comp_actor = static_cast<const ComplexActor*>(actor);
		action = comp_actor->GetAction();
	}
	const std::vector<Sprite*>& children 
		= VI_DOWNCASTING<ComplexSymbol*>(m_sym)->GetActionChildren(action);
	for (int i = 0, n = children.size(); i < n; ++i) {
		Sprite* child = children[i];
		if (child->SetFrame(frame, cpath)) {
			dirty = true;
		}
	}
	return dirty;
}

Sprite* ComplexSprite::FetchChild(const std::string& name, const SprTreePath& path) const
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

Sprite* ComplexSprite::FetchChild(int idx, const SprTreePath& path) const
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
	const Actor* actor = QueryActor(params.path);
	if (actor) {
		const ComplexActor* comp_actor = static_cast<const ComplexActor*>(actor);
		action = comp_actor->GetAction();
	}
	const std::vector<Sprite*>& children 
		= VI_DOWNCASTING<ComplexSymbol*>(m_sym)->GetActionChildren(action);
	if (visitor.GetOrder()) {
		for (int i = 0, n = children.size(); i < n; ++i) {
			if (!SpriteVisitor::VisitChild(visitor, params, children[i], ret)) {
				break;
			}
		}
	} else {
		for (int i = children.size() - 1; i >= 0; --i) {
			if (!SpriteVisitor::VisitChild(visitor, params, children[i], ret)) {
				break;
			}
		}
	}
	return ret;
}


}