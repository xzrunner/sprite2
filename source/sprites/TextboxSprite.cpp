#include "TextboxSprite.h"
#include "TextboxSymbol.h"

#include <gtxt_label.h>

#include <stddef.h>
#include <assert.h>

namespace s2
{

TextboxSprite::TextboxSprite()
	: m_time(0)
{
}

TextboxSprite::TextboxSprite(Symbol* sym, uint32_t id)
	: Sprite(sym, id)
	, m_time(0)
{
	TextboxSymbol* tb_sym = VI_DOWNCASTING<TextboxSymbol*>(sym);
	assert(tb_sym);
	m_tb = tb_sym->GetTextbox();
}

TextboxSprite* TextboxSprite::Clone() const
{
	return new TextboxSprite(*this);
}

static void
_init_gtxt_label_style(struct gtxt_label_style* dst, const Textbox& src) {
	dst->width  = src.width;
	dst->height = src.height;

	dst->align_h = src.align_hori;
	dst->align_v = src.align_vert;

	dst->space_h = src.space_hori;
	dst->space_v = src.space_vert;

	dst->gs.font = src.font_type;
	dst->gs.font_size = src.font_size;
	dst->gs.font_color.integer = src.font_color.ToRGBA();

	dst->gs.edge = src.has_edge;
	dst->gs.edge_size = src.edge_size;
	dst->gs.edge_color.integer = src.edge_color.ToRGBA();

	dst->overflow = src.overflow;
}

void TextboxSprite::SetText(const std::string& text) 
{ 
	m_text = text;

	struct gtxt_label_style style;
	_init_gtxt_label_style(&style, m_tb);
	gtxt_get_label_size(m_text.c_str(), &style, &m_size.x, &m_size.y);
}

}