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
	ProxySymbol(const std::vector<Sprite*>& children, const Actor* parent);
	virtual ~ProxySymbol();
	
	/**
	 *  @interface
	 *    Symbol
	 */
	virtual int Type() const;
	virtual void Traverse(const SymbolVisitor& visitor) {}
	virtual void Draw(const RenderParams& rp, const Sprite* spr = NULL) const;

	const std::vector<Sprite*>& GetChildren() const { return m_children; }

	const Actor* GetParent() const { return m_parent; }

protected:
	virtual sm::rect GetBoundingImpl(const Sprite* spr = NULL, const Actor* actor = NULL, bool cache = true) const;

private:
	const std::vector<Sprite*> m_children;

	const Actor* m_parent;

}; // ProxySymbol

}

#endif // _SPRITE2_PROXY_SYMBOL_H_
