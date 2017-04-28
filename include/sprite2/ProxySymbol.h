#ifndef _SPRITE2_PROXY_SYMBOL_H_
#define _SPRITE2_PROXY_SYMBOL_H_

#include "S2_Symbol.h"

#include <vector>

namespace s2
{

class Sprite;

class ProxySymbol : public VIRTUAL_INHERITANCE Symbol
{
public:
	ProxySymbol(const std::vector<std::pair<const Actor*, Sprite*> >& items);
	virtual ~ProxySymbol();
	
	/**
	 *  @interface
	 *    Symbol
	 */
	virtual int Type() const;
	virtual void Traverse(const SymbolVisitor& visitor) {}
	virtual void Draw(const RenderParams& rp, const Sprite* spr = NULL) const;

	const std::vector<std::pair<const Actor*, Sprite*> >& GetItems() const { return m_items; }

protected:
	virtual sm::rect GetBoundingImpl(const Sprite* spr = NULL, const Actor* actor = NULL, bool cache = true) const;

private:
	std::vector<std::pair<const Actor*, Sprite*> > m_items;

}; // ProxySymbol

}

#endif // _SPRITE2_PROXY_SYMBOL_H_
