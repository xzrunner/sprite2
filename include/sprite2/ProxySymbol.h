#ifndef _SPRITE2_PROXY_SYMBOL_H_
#define _SPRITE2_PROXY_SYMBOL_H_

#include "S2_Symbol.h"

#include <vector>
#include <memory>

namespace s2
{

class Sprite;

class ProxySymbol : public VIRTUAL_INHERITANCE Symbol
{
public:
	ProxySymbol(const std::vector<std::pair<const ActorConstPtr, SprPtr>>& items);
	
	/**
	 *  @interface
	 *    Symbol
	 */
	virtual int Type() const;
	virtual void Traverse(const SymbolVisitor& visitor) {}
	virtual RenderReturn DrawTree(const RenderParams& rp, const SprConstPtr& spr = nullptr) const;
	virtual RenderReturn DrawNode(cooking::DisplayList* dlist, const RenderParams& rp, const SprConstPtr& spr, ft::FTList& ft, int pos) const { return RENDER_SKIP; }

	auto& GetItems() const { return m_items; }

protected:
	virtual sm::rect GetBoundingImpl(const SprConstPtr& spr = nullptr, const ActorConstPtr& actor = nullptr, bool cache = true) const;

private:
	std::vector<std::pair<const ActorConstPtr, SprPtr>> m_items;

}; // ProxySymbol

}

#endif // _SPRITE2_PROXY_SYMBOL_H_
