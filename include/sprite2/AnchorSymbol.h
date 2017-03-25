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
	virtual void Traverse(const SymbolVisitor& visitor);
	virtual void Draw(const RenderParams& rp, const Sprite* spr = NULL) const;
	virtual bool Update(const RenderParams& rp, float time);
	virtual sm::rect GetBounding(const Sprite* spr = NULL, const Actor* actor = NULL) const;

}; // AnchorSymbol

}

#endif // _SPRITE2_ANCHOR_SYMBOL_H_