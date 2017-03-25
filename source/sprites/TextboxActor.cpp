#include "TextboxActor.h"
#include "TextboxSprite.h"

#include <gtxt_label.h>

namespace s2
{


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

void TextboxActor::SetText(const std::string& text)
{
	m_text = text;

	const TextboxSprite* text_spr = VI_DOWNCASTING<const TextboxSprite*>(GetSpr());

	struct gtxt_label_style style;
	_init_gtxt_label_style(&style, text_spr->GetTextbox());
	gtxt_get_label_size(m_text.c_str(), &style, &m_size.x, &m_size.y);

	GetSpr()->SetBoundingDirty(true);
}

}