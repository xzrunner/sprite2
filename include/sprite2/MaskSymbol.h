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
	virtual RenderReturn DrawTree(const RenderParams& rp, const Sprite* spr = nullptr) const;
	virtual RenderReturn DrawNode(cooking::DisplayList* dlist, const RenderParams& rp, const Sprite* spr, ft::FTList& ft, int pos) const;

	const SprPtr& GetBase() const { return m_base; }
	void SetBase(const SprPtr& base) { m_base = base; }

	const SprPtr& GetMask() const { return m_mask; }
	void SetMask(const SprPtr& mask) { m_mask = mask; }

protected:
	virtual sm::rect GetBoundingImpl(const Sprite* spr = nullptr, const Actor* actor = nullptr, bool cache = true) const;

protected:
	SprPtr m_base;
	SprPtr m_mask;

}; // MaskSymbol

}

#endif // _SPRITE2_MASK_SYMBOL_H_