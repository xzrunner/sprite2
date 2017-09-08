#ifndef _SPRITE2_MASK_SYMBOL_H_
#define _SPRITE2_MASK_SYMBOL_H_

#include "S2_Symbol.h"

namespace s2
{

class MaskSymbol : public VIRTUAL_INHERITANCE Symbol
{
public:
	MaskSymbol();
	MaskSymbol(uint32_t id);
	virtual ~MaskSymbol();

	/**
	 *  @interface
	 *    Symbol
	 */
	virtual int Type() const;
	virtual void Traverse(const SymbolVisitor& visitor);
	virtual RenderReturn Draw(const RenderParams& rp, const Sprite* spr = NULL) const;
	virtual RenderReturn DrawDeferred(cooking::DisplayList* dlist, const RenderParams& rp, const Sprite* spr = NULL) const;

	const Sprite* GetBase() const { return m_base; }
	void SetBase(Sprite* base);

	const Sprite* GetMask() const { return m_mask; }
	void SetMask(Sprite* mask);

protected:
	virtual sm::rect GetBoundingImpl(const Sprite* spr = NULL, const Actor* actor = NULL, bool cache = true) const;

protected:
	Sprite* m_base;
	Sprite* m_mask;

}; // MaskSymbol

}

#endif // _SPRITE2_MASK_SYMBOL_H_