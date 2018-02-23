#include "sprite2/ProxySymbol.h"
#include "sprite2/SymType.h"
#include "sprite2/Sprite.h"
#include "sprite2/Actor.h"
#include "sprite2/DrawNode.h"

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

pt2::RenderReturn ProxySymbol::DrawTree(cooking::DisplayList* dlist, const RenderParams& rp, const Sprite* spr) const
{
	pt2::RenderReturn ret = pt2::RENDER_OK;
	for (auto& item : m_items) {
		ret |= DrawNode::Draw(dlist, item.second.get(), rp);
	}
	return ret;
}

sm::rect ProxySymbol::GetBoundingImpl(const Sprite* spr, const Actor* actor, bool cache) const
{
	return sm::rect(); // empty
}

}