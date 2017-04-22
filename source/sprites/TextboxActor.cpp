#include "TextboxActor.h"
#include "TextboxSprite.h"

#include <gtxt_label.h>

#include <S2_Sprite.h>
#include <S2_Symbol.h>

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
	if (m_text == text) {
		return;
	}

	m_text = text;

	const TextboxSprite* text_spr = VI_DOWNCASTING<const TextboxSprite*>(GetSpr());

	const Textbox& tb = text_spr->GetTextbox();

	struct gtxt_label_style style;
	_init_gtxt_label_style(&style, tb);
	float w, h;
	gtxt_get_label_size(m_text.c_str(), &style, &w, &h);

	sm::rect rect = GetSpr()->GetSymbol()->GetBounding();

	switch (tb.align_hori)
	{
	case Textbox::HA_LEFT:
		rect.xmax = rect.xmin + w;
		break;
	case Textbox::HA_RIGHT:
		rect.xmin = rect.xmax - w;
		break;
	case Textbox::HA_CENTER:
		rect.xmin = (rect.xmin + rect.xmax) * 0.5f - w * 0.5f;
		rect.xmax = rect.xmin + w;
		break;
	}
	switch (tb.align_vert)
	{
	case Textbox::VA_TOP:
		rect.ymin = rect.ymax - h;
		break;
	case Textbox::VA_BOTTOM:
		rect.ymax = rect.ymin + h;
		break;
	case Textbox::VA_CENTER:
		rect.ymin = (rect.ymin + rect.ymax) * 0.5f - h * 0.5f;
		rect.ymax = rect.ymin + h;
		break;
	}

	ActorAABB& aabb = GetAABB();
	aabb.SetRect(rect);
	aabb.UpdateParent(this);

	SetFlattenDirtyToRoot();
}

}