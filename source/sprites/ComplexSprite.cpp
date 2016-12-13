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
	const std::vector<Sprite*>& children = VI_DOWNCASTING<ComplexSymbol*>(m_sym)->GetChildren();
	for (int i = 0, n = children.size(); i < n; ++i) {
		children[i]->OnMessage(msg);
	}
}

bool ComplexSprite::Update(const RenderParams& params)
{
	RenderParams p = params;
	p.mt = GetLocalMat() * params.mt;

	bool dirty = false;
	const std::vector<Sprite*>& children = VI_DOWNCASTING<ComplexSymbol*>(m_sym)->GetChildren();
	for (int i = 0, n = children.size(); i < n; ++i) {
		if (children[i]->Update(p)) {
			dirty = true;
		}
	}
	return dirty;
}

Sprite* ComplexSprite::FetchChild(const std::string& name) const
{
	const std::vector<Sprite*>& children = VI_DOWNCASTING<ComplexSymbol*>(m_sym)->GetChildren();
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
	const std::vector<Sprite*>& children = VI_DOWNCASTING<ComplexSymbol*>(m_sym)->GetChildren();
	if (idx >= 0 && idx < children.size()) {
		return children[idx];
	} else {
		return NULL;
	}
}

void ComplexSprite::MountChild(const std::string& name, Sprite* child)
{
	VI_DOWNCASTING<ComplexSymbol*>(m_sym)->Change(name, child);
}

void ComplexSprite::SetAction(const std::string& name)
{
	m_action = VI_DOWNCASTING<ComplexSymbol*>(m_sym)->GetActionIdx(name);
}

bool ComplexSprite::TraverseChildren(SprVisitor& visitor, const sm::mat4* mat) const
{
	const std::vector<Sprite*>& children = VI_DOWNCASTING<ComplexSymbol*>(m_sym)->GetChildren();
	if (visitor.GetOrder()) {
		for (int i = 0, n = children.size(); i < n; ++i) {
			if (!children[i]->Traverse(visitor, mat)) {
				return false;
			}
		}
	} else {
		for (int i = children.size() - 1; i >= 0; --i) {
			if (!children[i]->Traverse(visitor, mat)) {
				return false;
			}
		}
	}
	return true;
}

}