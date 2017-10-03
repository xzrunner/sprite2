#include "ProxySymbol.h"
#include "SymType.h"
#include "S2_Sprite.h"
#include "S2_Actor.h"
#include "DrawNode.h"

#include <assert.h>

namespace s2
{

ProxySymbol::ProxySymbol(const std::vector<std::pair<const ActorConstPtr, SprPtr>>& items)
	: m_items(items)
{
}

int ProxySymbol::Type() const
{
	return SYM_PROXY;
}

RenderReturn ProxySymbol::DrawTree(const RenderParams& rp, const SprConstPtr& spr) const
{
	RenderReturn ret = RENDER_OK;
	for (int i = 0, n = m_items.size(); i < n; ++i) {
		ret |= DrawNode::Draw(m_items[i].second, rp);
	}
	return ret;
}

sm::rect ProxySymbol::GetBoundingImpl(const SprConstPtr& spr, const ActorConstPtr& actor, bool cache) const
{
	return sm::rect(); // empty
}

}