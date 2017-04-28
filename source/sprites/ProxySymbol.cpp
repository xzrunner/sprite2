#include "ProxySymbol.h"
#include "SymType.h"
#include "S2_Sprite.h"
#include "S2_Actor.h"
#include "DrawNode.h"

#include <assert.h>

namespace s2
{

ProxySymbol::ProxySymbol(const std::vector<std::pair<const Actor*, Sprite*> >& items)
	: m_items(items)
{
	for (int i=  0, n = m_items.size(); i < n; ++i) 
	{
		Sprite* child = m_items[i].second;
		assert(child);
		child->AddReference();
		const Actor* parent = m_items[i].first;
		if (parent) {
			parent->GetSpr()->AddReference();
		}
	}
}

ProxySymbol::~ProxySymbol()
{
	for (int i = 0, n = m_items.size(); i < n; ++i) {
		m_items[i].second->RemoveReference();
		const Actor* parent = m_items[i].first;
		if (parent) {
			parent->GetSpr()->RemoveReference();
		}
	}
}

int ProxySymbol::Type() const
{
	return SYM_PROXY;
}

void ProxySymbol::Draw(const RenderParams& rp, const Sprite* spr) const
{
	for (int i = 0, n = m_items.size(); i < n; ++i) {
		DrawNode::Draw(m_items[i].second, rp);
	}
}

sm::rect ProxySymbol::GetBoundingImpl(const Sprite* spr, const Actor* actor, bool cache) const
{
	return sm::rect();
}

}