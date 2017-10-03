#ifndef _SPRITE2_ANCHOR_SYMBOL_H_
#define _SPRITE2_ANCHOR_SYMBOL_H_

#include "S2_Symbol.h"

#include <memory>

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
	virtual RenderReturn DrawTree(const RenderParams& rp, const SprConstPtr& spr) const;
	virtual RenderReturn DrawNode(cooking::DisplayList* dlist, const RenderParams& rp, const SprConstPtr& spr, ft::FTList& ft, int pos) const;

protected:
	virtual sm::rect GetBoundingImpl(const SprConstPtr& spr = nullptr, const ActorConstPtr& actor = nullptr, bool cache = true) const;

private:
	static ActorConstPtr GetRealActor(const SprConstPtr& spr, 
		const ActorConstPtr& actor);

}; // AnchorSymbol

}

#endif // _SPRITE2_ANCHOR_SYMBOL_H_