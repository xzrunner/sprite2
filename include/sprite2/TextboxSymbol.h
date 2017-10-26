#ifndef _SPRITE2_TEXTBOX_SYMBOL_H_
#define _SPRITE2_TEXTBOX_SYMBOL_H_

#include "sprite2/Symbol.h"
#include "sprite2/Textbox.h"
#include "sprite2/pre_defined.h"

#include S2_MAT_HEADER

#include <cu/cu_stl.h>

#include <stdint.h>

struct gtxt_label_style;

namespace s2
{

class TextboxSymbol : public VIRTUAL_INHERITANCE Symbol
{
public:
	TextboxSymbol();
	TextboxSymbol(uint32_t id);
	TextboxSymbol(Textbox tb);
	virtual ~TextboxSymbol();

	/**
	 *  @interface
	 *    Symbol
	 */
	virtual int Type() const override;
	virtual void Traverse(const SymbolVisitor& visitor) override {}
	virtual RenderReturn DrawTree(cooking::DisplayList* dlist, const RenderParams& rp, const Sprite* spr = nullptr) const override;
#ifndef S2_DISABLE_FLATTEN
	virtual RenderReturn DrawNode(cooking::DisplayList* dlist, const RenderParams& rp, const Sprite* spr, ft::FTList& ft, int pos) const override;
#endif // S2_DISABLE_FLATTEN

	const Textbox& GetTextbox() const { return m_tb; }
	Textbox& GetTextbox() { return m_tb; }

	void SetTextbox(const Textbox& tb) { m_tb = tb; }

protected:
	virtual sm::rect GetBoundingImpl(const Sprite* spr = nullptr, const Actor* actor = nullptr, bool cache = true) const;

	virtual void DrawText(cooking::DisplayList* dlist, const gtxt_label_style& style, const S2_MAT& mt, const Color& mul,
		const Color& add, const CU_STR& text, int time, bool richtext) const = 0;

private:
	RenderReturn DrawImpl(cooking::DisplayList* dlist, const RenderParams& rp, const Sprite* spr = nullptr) const;

protected:
	Textbox m_tb;

}; // TextboxSymbol

}

#endif // _SPRITE2_TEXTBOX_SYMBOL_H_