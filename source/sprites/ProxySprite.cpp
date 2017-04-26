#include "ProxySprite.h"
#include "ProxySymbol.h"
#include "ProxyHelper.h"

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
	const std::vector<Sprite*>& children 
		= VI_DOWNCASTING<ProxySymbol*>(m_sym)->GetChildren();
	for (int i = 0, n = children.size(); i < n; ++i) {
		children[i]->OnMessage(up, msg);
	}
}

bool ProxySprite::Update(const UpdateParams& up)
{
	bool ret = false;
	const std::vector<Sprite*>& children 
		= VI_DOWNCASTING<ProxySymbol*>(m_sym)->GetChildren();
	for (int i = 0, n = children.size(); i < n; ++i) {
		if (children[i]->Update(up)) {
			ret = true;
		}
	}
	return ret;
}

bool ProxySprite::SetFrame(const UpdateParams& up, int frame, bool force)
{
	bool ret = false;
	const std::vector<Sprite*>& children 
		= VI_DOWNCASTING<ProxySymbol*>(m_sym)->GetChildren();
	for (int i = 0, n = children.size(); i < n; ++i) {
		if (children[i]->SetFrame(up, frame, force)) {
			ret = true;
		}
	}
	return ret;
}

Sprite* ProxySprite::FetchChild(const std::string& name, const Actor* actor) const
{
	std::vector<Sprite*> group;
	const std::vector<Sprite*>& children 
		= VI_DOWNCASTING<ProxySymbol*>(m_sym)->GetChildren();
	for (int i = 0, n = children.size(); i < n; ++i) {
		Sprite* spr = children[i]->FetchChild(name, actor);
		if (spr) {
			group.push_back(spr);
		}
	}
	return ProxyHelper::BuildGroup(group, actor);
}

Sprite* ProxySprite::FetchChild(int idx, const Actor* actor) const
{
	std::vector<Sprite*> group;
	const std::vector<Sprite*>& children 
		= VI_DOWNCASTING<ProxySymbol*>(m_sym)->GetChildren();
	for (int i = 0, n = children.size(); i < n; ++i) {
		Sprite* spr = children[i]->FetchChild(idx, actor);
		if (spr) {
			group.push_back(spr);
		}
	}
	return ProxyHelper::BuildGroup(group, actor);
}

void ProxySprite::SetSymbol(Symbol* sym)
{
	const std::vector<Sprite*>& children 
		= VI_DOWNCASTING<ProxySymbol*>(m_sym)->GetChildren();
	for (int i = 0, n = children.size(); i < n; ++i) {
		children[i]->SetSymbol(sym);
	}
}

void ProxySprite::SetCenter(const sm::vec2& pos)
{
	const std::vector<Sprite*>& children 
		= VI_DOWNCASTING<ProxySymbol*>(m_sym)->GetChildren();
	for (int i = 0, n = children.size(); i < n; ++i) {
		children[i]->SetCenter(pos);
	}
}

void ProxySprite::SetPosition(const sm::vec2& pos)
{
	const std::vector<Sprite*>& children 
		= VI_DOWNCASTING<ProxySymbol*>(m_sym)->GetChildren();
	for (int i = 0, n = children.size(); i < n; ++i) {
		children[i]->SetPosition(pos);
	}
}

void ProxySprite::SetAngle(float angle)
{
	const std::vector<Sprite*>& children 
		= VI_DOWNCASTING<ProxySymbol*>(m_sym)->GetChildren();
	for (int i = 0, n = children.size(); i < n; ++i) {
		children[i]->SetAngle(angle);
	}
}

void ProxySprite::SetScale(const sm::vec2& scale)
{
	const std::vector<Sprite*>& children 
		= VI_DOWNCASTING<ProxySymbol*>(m_sym)->GetChildren();
	for (int i = 0, n = children.size(); i < n; ++i) {
		children[i]->SetScale(scale);
	}
}

void ProxySprite::SetShear(const sm::vec2& shear)
{
	const std::vector<Sprite*>& children 
		= VI_DOWNCASTING<ProxySymbol*>(m_sym)->GetChildren();
	for (int i = 0, n = children.size(); i < n; ++i) {
		children[i]->SetShear(shear);
	}
}

void ProxySprite::SetOffset(const sm::vec2& offset)
{
	const std::vector<Sprite*>& children 
		= VI_DOWNCASTING<ProxySymbol*>(m_sym)->GetChildren();
	for (int i = 0, n = children.size(); i < n; ++i) {
		children[i]->SetOffset(offset);
	}
}

VisitResult ProxySprite::TraverseChildren(SpriteVisitor& visitor, const SprVisitorParams& params) const
{
	const std::vector<Sprite*>& children 
		= VI_DOWNCASTING<ProxySymbol*>(m_sym)->GetChildren();
	for (int i = 0, n = children.size(); i < n; ++i) {
		VisitResult ret = children[i]->TraverseChildren(visitor, params);
		if (ret == VISIT_STOP) {
			return ret;
		}
	}
	return VISIT_OVER;
}

} 