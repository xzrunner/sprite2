#include "ProxySymbol.h"
#include "SymType.h"
#include "S2_Sprite.h"
#include "S2_Actor.h"
#include "DrawNode.h"

namespace s2
{

ProxySymbol::ProxySymbol(const std::vector<Sprite*>& children, const Actor* parent)
	: m_children(children)
	, m_parent(parent)
{
	for_each(m_children.begin(), m_children.end(), cu::AddRefFunctor<Sprite>());
	if (parent) {
		parent->GetSpr()->AddReference();
	}
}

ProxySymbol::~ProxySymbol()
{
	for_each(m_children.begin(), m_children.end(), cu::RemoveRefFunctor<Sprite>());
	if (m_parent) {
		m_parent->GetSpr()->RemoveReference();
	}
}

int ProxySymbol::Type() const
{
	return SYM_PROXY;
}

void ProxySymbol::Draw(const RenderParams& rp, const Sprite* spr) const
{
	for (int i = 0, n = m_children.size(); i < n; ++i) {
		DrawNode::Draw(m_children[i], rp);
	}
}

sm::rect ProxySymbol::GetBoundingImpl(const Sprite* spr, const Actor* actor, bool cache) const
{
	return sm::rect();
}

}