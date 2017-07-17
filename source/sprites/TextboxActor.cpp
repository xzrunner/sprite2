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
	sm::rect rect = CalcAABB(tb, GetSpr()->GetSymbol()->GetBounding(), m_text);

	ActorAABB& aabb = GetAABB();
	aabb.SetRect(rect);
	aabb.UpdateParent(this);

	SetFlattenDirtyToRoot();
}

sm::rect TextboxActor::CalcAABB(const Textbox& tb, const sm::rect& rect, const std::string& text)
{
	sm::rect ret = rect;

	struct gtxt_label_style style;
	_init_gtxt_label_style(&style, tb);
	float w, h;
	gtxt_get_label_size(text.c_str(), &style, &w, &h);

	switch (tb.align_hori)
	{
	case Textbox::HA_LEFT:
		ret.xmax = rect.xmin + w;
		break;
	case Textbox::HA_RIGHT:
		ret.xmin = rect.xmax - w;
		break;
	case Textbox::HA_CENTER:
		ret.xmin = (rect.xmin + rect.xmax) * 0.5f - w * 0.5f;
		ret.xmax = ret.xmin + w;
		break;
	}
	switch (tb.align_vert)
	{
	case Textbox::VA_TOP:
		ret.ymin = rect.ymax - h;
		break;
	case Textbox::VA_BOTTOM:
		ret.ymax = rect.ymin + h;
		break;
	case Textbox::VA_CENTER:
		ret.ymin = (rect.ymin + rect.ymax) * 0.5f - h * 0.5f;
		ret.ymax = ret.ymin + h;
		break;
	}

	return ret;
}

}