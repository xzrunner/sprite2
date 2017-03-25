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
	TextboxSymbol() {}
	TextboxSymbol(uint32_t id);
	TextboxSymbol(Textbox tb);

	/**
	 *  @interface
	 *    Symbol
	 */
	virtual int Type() const;
	virtual void Traverse(const SymbolVisitor& visitor) {}
	virtual void Draw(const RenderParams& rp, const Sprite* spr = NULL) const;
	virtual sm::rect GetBounding(const Sprite* spr = NULL, const Actor* actor = NULL) const;

	const Textbox& GetTextbox() const { return m_tb; }
	Textbox& GetTextbox() { return m_tb; }

protected:
	virtual void DrawText(const gtxt_label_style& style, const S2_MAT& mt, const Color& mul, 
		const Color& add, const std::string& text, int time, bool richtext) const = 0;

protected:
	Textbox m_tb;

}; // TextboxSymbol

}

#endif // _SPRITE2_TEXTBOX_SYMBOL_H_