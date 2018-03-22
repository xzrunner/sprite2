#include "sprite2/TextboxActor.h"
#include "sprite2/TextboxSprite.h"
#include "sprite2/Sprite.h"
#include "sprite2/Symbol.h"

#include <gtxt_label.h>

namespace s2
{

static void
_init_gtxt_label_style(struct gtxt_label_style* dst, const pt2::Textbox& src) {
	dst->width  = src.width;
	dst->height = src.height;

	dst->align_h = src.align_hori;
	dst->align_v = src.align_vert;

	dst->space_h = src.space_hori;
	dst->space_v = src.space_vert;

	dst->gs.font = src.font_type;
	dst->gs.font_size = src.font_size;
	dst->gs.font_color.mode_type = 0;
	dst->gs.font_color.mode.ONE.color.integer = src.font_color.ToRGBA();

	dst->gs.edge = src.has_edge;
	dst->gs.edge_size = src.edge_size;
	dst->gs.edge_color.mode_type = 0;
	dst->gs.edge_color.mode.ONE.color.integer = src.edge_color.ToRGBA();

	dst->overflow = src.overflow;
}

void TextboxActor::SetText(const CU_STR& text)
{
	if (m_text == text) {
		return;
	}

	m_text = text;

	auto text_spr = S2_VI_DOWN_CAST<const TextboxSprite*>(GetSprRaw());
	const pt2::Textbox& tb = text_spr->GetTextbox();
	sm::rect rect = CalcAABB(tb, GetSprRaw()->GetSymbol()->GetBounding(), m_text);

	ActorAABB& aabb = GetAABB();
	aabb.SetRect(rect);
	aabb.UpdateParent(this);
}

sm::rect TextboxActor::CalcAABB(const pt2::Textbox& tb, const sm::rect& rect, const CU_STR& text)
{
	sm::rect ret = rect;

	struct gtxt_label_style style;
	_init_gtxt_label_style(&style, tb);
	float w, h;
	gtxt_get_label_size(text.c_str(), &style, &w, &h);

	switch (tb.align_hori)
	{
	case pt2::Textbox::HA_LEFT:
		ret.xmax = rect.xmin + w;
		break;
	case pt2::Textbox::HA_RIGHT:
		ret.xmin = rect.xmax - w;
		break;
	case pt2::Textbox::HA_CENTER:
		ret.xmin = (rect.xmin + rect.xmax) * 0.5f - w * 0.5f;
		ret.xmax = ret.xmin + w;
		break;
	}
	switch (tb.align_vert)
	{
	case pt2::Textbox::VA_TOP:
		ret.ymin = rect.ymax - h;
		break;
	case pt2::Textbox::VA_BOTTOM:
		ret.ymax = rect.ymin + h;
		break;
	case pt2::Textbox::VA_CENTER:
		ret.ymin = (rect.ymin + rect.ymax) * 0.5f - h * 0.5f;
		ret.ymax = ret.ymin + h;
		break;
	}

	return ret;
}

}