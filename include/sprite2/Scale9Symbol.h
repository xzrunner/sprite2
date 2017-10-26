#ifndef _SPRITE2_SCALE9_SYMBOL_H_
#define _SPRITE2_SCALE9_SYMBOL_H_

#include "S2_Symbol.h"
#include "Scale9.h"

namespace s2
{

class Scale9Symbol : public VIRTUAL_INHERITANCE Symbol
{
public:
	Scale9Symbol();
	Scale9Symbol(uint32_t id);
	virtual ~Scale9Symbol();

	/**
	 *  @interface
	 *    Symbol
	 */
	virtual int Type() const override;
	virtual void Traverse(const SymbolVisitor& visitor) override;
	virtual RenderReturn DrawTree(cooking::DisplayList* dlist, const RenderParams& rp, const Sprite* spr = nullptr) const override;
#ifndef S2_DISABLE_FLATTEN
	virtual RenderReturn DrawNode(cooking::DisplayList* dlist, const RenderParams& rp, const Sprite* spr, ft::FTList& ft, int pos) const override { return RENDER_SKIP; }
#endif // S2_DISABLE_FLATTEN

	const Scale9& GetScale9() const { return m_s9; }
	Scale9& GetScale9() { return m_s9; }

	void Resize(float width, float height);

protected:
	virtual sm::rect GetBoundingImpl(const Sprite* spr = nullptr, const Actor* actor = nullptr, bool cache = true) const;

protected:
	Scale9 m_s9;

}; // Scale9Symbol

}

#endif // _SPRITE2_SCALE9_SYMBOL_H_	