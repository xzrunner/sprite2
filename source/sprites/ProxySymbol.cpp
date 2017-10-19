#include "ProxySymbol.h"
#include "SymType.h"
#include "S2_Sprite.h"
#include "S2_Actor.h"
#include "DrawNode.h"

#include <assert.h>

namespace s2
{

ProxySymbol::ProxySymbol(const CU_VEC<std::pair<const ActorConstPtr, SprPtr>>& items)
	: m_items(items)
{
}

int ProxySymbol::Type() const
{
	return SYM_PROXY;
}

RenderReturn ProxySymbol::DrawTree(const RenderParams& rp, const Sprite* spr) const
{
	RenderReturn ret = RENDER_OK;
	for (auto& item : m_items) {
		ret |= DrawNode::Draw(item.second.get(), rp);
	}
	return ret;
}

sm::rect ProxySymbol::GetBoundingImpl(const Sprite* spr, const Actor* actor, bool cache) const
{
	return sm::rect(); // empty
}

}