#ifndef _SPRITE2_ANCHOR_SYMBOL_H_
#define _SPRITE2_ANCHOR_SYMBOL_H_

#include "S2_Symbol.h"

#include <stdint.h>

namespace s2
{

class AnchorSymbol : public VIRTUAL_INHERITANCE Symbol
{
public:
	AnchorSymbol() {}

	/**
	 *  @interface
	 *    Symbol
	 */
	virtual int Type() const;
	virtual void Traverse(const SymbolVisitor& visitor) {}
	virtual RenderReturn DrawTree(const RenderParams& rp, const Sprite* spr = NULL) const;
	virtual RenderReturn DrawNode(cooking::DisplayList* dlist, const RenderParams& rp, const Sprite* spr, ft::FTList& ft, int pos) const;

protected:
	virtual sm::rect GetBoundingImpl(const Sprite* spr = NULL, const Actor* actor = NULL, bool cache = true) const;

private:
	static const Actor* GetRealActor(const Sprite* spr, const Actor* actor);

}; // AnchorSymbol

}

#endif // _SPRITE2_ANCHOR_SYMBOL_H_