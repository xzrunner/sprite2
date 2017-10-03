#ifndef _SPRITE2_TEXTBOX_SYMBOL_H_
#define _SPRITE2_TEXTBOX_SYMBOL_H_

#include "S2_Symbol.h"
#include "Textbox.h"
#include "pre_defined.h"

#include S2_MAT_HEADER

#include <string>

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
	virtual int Type() const;
	virtual void Traverse(const SymbolVisitor& visitor) {}
	virtual RenderReturn DrawTree(const RenderParams& rp, const SprConstPtr& spr = nullptr) const;
	virtual RenderReturn DrawNode(cooking::DisplayList* dlist, const RenderParams& rp, const SprConstPtr& spr, ft::FTList& ft, int pos) const;

	const Textbox& GetTextbox() const { return m_tb; }
	Textbox& GetTextbox() { return m_tb; }

	void SetTextbox(const Textbox& tb) { m_tb = tb; }

protected:
	virtual sm::rect GetBoundingImpl(const SprConstPtr& spr = nullptr, const ActorConstPtr& actor = nullptr, bool cache = true) const;

	virtual void DrawText(cooking::DisplayList* dlist, const gtxt_label_style& style, const S2_MAT& mt, const Color& mul,
		const Color& add, const std::string& text, int time, bool richtext) const = 0;

private:
	RenderReturn DrawImpl(cooking::DisplayList* dlist, const RenderParams& rp, const SprConstPtr& spr = nullptr) const;

protected:
	Textbox m_tb;

}; // TextboxSymbol

}

#endif // _SPRITE2_TEXTBOX_SYMBOL_H_