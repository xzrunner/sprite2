#ifndef _SPRITE2_PROXY_SYMBOL_H_
#define _SPRITE2_PROXY_SYMBOL_H_

#include "sprite2/Symbol.h"

#include <cu/cu_stl.h>

#include <memory>

namespace s2
{

class Sprite;

class ProxySymbol : public VIRTUAL_INHERITANCE Symbol
{
public:
	ProxySymbol(const CU_VEC<std::pair<const ActorConstPtr, SprPtr>>& items);
	
	/**
	 *  @interface
	 *    Symbol
	 */
	virtual int Type() const override;
	virtual void Traverse(const SymbolVisitor& visitor) override {}
	virtual pt2::RenderReturn DrawTree(cooking::DisplayList* dlist, const RenderParams& rp, const Sprite* spr = nullptr) const override;
#ifndef S2_DISABLE_FLATTEN
	virtual pt2::RenderReturn DrawNode(cooking::DisplayList* dlist, const RenderParams& rp, const Sprite* spr, ft::FTList& ft, int pos) const override { return pt2::RENDER_SKIP; }
#endif // S2_DISABLE_FLATTEN

	auto& GetItems() const { return m_items; }

protected:
	virtual sm::rect GetBoundingImpl(const Sprite* spr = nullptr, const Actor* actor = nullptr, bool cache = true) const override;

private:
	CU_VEC<std::pair<const ActorConstPtr, SprPtr>> m_items;

}; // ProxySymbol

}

#endif // _SPRITE2_PROXY_SYMBOL_H_
