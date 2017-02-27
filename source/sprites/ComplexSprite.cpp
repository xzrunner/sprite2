#include "ComplexSprite.h"
#include "ComplexSymbol.h"
#include "RenderParams.h"
#include "SprVisitor.h"

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

void ComplexSprite::OnMessage(Message msg)
{
	const std::vector<Sprite*>& children 
		= VI_DOWNCASTING<ComplexSymbol*>(m_sym)->GetActionChildren(m_action);
	for (int i = 0, n = children.size(); i < n; ++i) {
		children[i]->OnMessage(msg);
	}
}

bool ComplexSprite::Update(const RenderParams& rp)
{
	RenderParams rp_child = rp;
	rp_child.mt = GetLocalMat() * rp.mt;
	rp_child.shader = GetShader() * rp.shader;
	rp_child.path.Push(GetID());

	bool dirty = false;
	const std::vector<Sprite*>& children 
		= VI_DOWNCASTING<ComplexSymbol*>(m_sym)->GetActionChildren(m_action);
	for (int i = 0, n = children.size(); i < n; ++i) 
	{
		const Sprite* spr = children[i];
		if (spr->IsHasProxy()) {
			const Sprite* proxy = spr->GetProxy(rp_child.path);
			if (proxy) {
				spr = proxy;
			}
		}
		if (const_cast<Sprite*>(spr)->Update(rp_child)) {
			dirty = true;
		}
	}
	return dirty;
}

bool ComplexSprite::SetFrame(int frame)
{
	bool dirty = false;
	const std::vector<Sprite*>& children 
		= VI_DOWNCASTING<ComplexSymbol*>(m_sym)->GetActionChildren(m_action);
	for (int i = 0, n = children.size(); i < n; ++i) {
		Sprite* spr = children[i];
		if (!spr->IsForceUpFrame() && !spr->GetName().empty()) {
			continue;
		}
		if (spr->SetFrame(frame)) {
			dirty = true;
		}
	}
	return dirty;
}

Sprite* ComplexSprite::FetchChild(const std::string& name) const
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

Sprite* ComplexSprite::FetchChild(int idx) const
{
	const std::vector<Sprite*>& children 
		= VI_DOWNCASTING<ComplexSymbol*>(m_sym)->GetAllChildren();
	if (idx >= 0 && idx < children.size()) {
		return children[idx];
	} else {
		return NULL;
	}
}

void ComplexSprite::SetAction(const std::string& name)
{
	m_action = VI_DOWNCASTING<ComplexSymbol*>(m_sym)->GetActionIdx(name);
}

bool ComplexSprite::TraverseChildren(SprVisitor& visitor, const SprVisitorParams& params) const
{
	const std::vector<Sprite*>& children 
		= VI_DOWNCASTING<ComplexSymbol*>(m_sym)->GetActionChildren(m_action);
	if (visitor.GetOrder()) {
		for (int i = 0, n = children.size(); i < n; ++i) {
			if (!children[i]->Traverse(visitor, params)) {
				return false;
			}
		}
	} else {
		for (int i = children.size() - 1; i >= 0; --i) {
			if (!children[i]->Traverse(visitor, params)) {
				return false;
			}
		}
	}
	return true;
}

}