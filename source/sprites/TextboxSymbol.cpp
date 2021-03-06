#include "sprite2/TextboxSymbol.h"
#include "sprite2/SymType.h"
#include "sprite2/TextboxSprite.h"
#include "sprite2/RenderParams.h"
#include "sprite2/TextboxActor.h"
#include "sprite2/DrawNode.h"
#include "sprite2/UpdateParams.h"
#include "sprite2/TextboxSprite.h"
#ifndef S2_DISABLE_STATISTICS
#include "sprite2/StatSymDraw.h"
#include "sprite2/StatSymCount.h"
#endif // S2_DISABLE_STATISTICS

#include <memmgr/Allocator.h>
#include <gtxt_label.h>
#include <shaderlab/Blackboard.h>
#include <shaderlab/ShaderMgr.h>
#include <shaderlab/RenderContext.h>
#include <shaderlab/FilterMode.h>
#include <shaderlab/FilterShader.h>
#ifndef S2_DISABLE_DEFERRED
#include <cooking/DisplayList.h>
#endif // S2_DISABLE_DEFERRED

#include <assert.h>

namespace s2
{

TextboxSymbol::TextboxSymbol()
{
#ifndef S2_DISABLE_STATISTICS
	StatSymCount::Instance()->Add(STAT_SYM_TEXTBOX);
#endif // S2_DISABLE_STATISTICS
}

TextboxSymbol::TextboxSymbol(uint32_t id)
	: Symbol(id)
{
#ifndef S2_DISABLE_STATISTICS
	StatSymCount::Instance()->Add(STAT_SYM_TEXTBOX);
#endif // S2_DISABLE_STATISTICS
}

TextboxSymbol::TextboxSymbol(pt2::Textbox tb)
	: m_tb(tb)
{
#ifndef S2_DISABLE_STATISTICS
	StatSymCount::Instance()->Add(STAT_SYM_TEXTBOX);
#endif // S2_DISABLE_STATISTICS
}

TextboxSymbol::~TextboxSymbol()
{
#ifndef S2_DISABLE_STATISTICS
	StatSymCount::Instance()->Add(STAT_SYM_TEXTBOX);
#endif // S2_DISABLE_STATISTICS
}

int TextboxSymbol::Type() const 
{ 
	return SYM_TEXTBOX; 
}

pt2::RenderReturn TextboxSymbol::DrawTree(cooking::DisplayList* dlist, const RenderParams& rp, const Sprite* spr) const
{
	if (!spr) {
		return pt2::RENDER_NO_DATA;
	}

	RenderParamsProxy rp_proxy;
	RenderParams* rp_child = rp_proxy.obj;
	memcpy(rp_child, &rp, sizeof(rp));

	if (!DrawNode::Prepare(rp, spr, *rp_child)) {
		return pt2::RENDER_INVISIBLE;
	}

	pt2::RenderReturn ret = DrawImpl(dlist, *rp_child, spr);

	return ret;
}

#ifndef S2_DISABLE_FLATTEN
pt2::RenderReturn TextboxSymbol::DrawNode(cooking::DisplayList* dlist, const RenderParams& rp, const Sprite* spr, ft::FTList& ft, int pos) const
{
	assert(spr);
	return DrawImpl(dlist, rp, spr);
}
#endif // S2_DISABLE_FLATTEN

void TextboxSymbol::CopyColor(gtxt_glyph_color& dst, const pt2::GradientColor& src)
{
	if (src.items.size() == 3)
	{
		dst.mode_type = 2;
		dst.mode.THREE.begin_col.integer = src.items[0].col.ToRGBA();
		dst.mode.THREE.mid_col.integer = src.items[1].col.ToRGBA();
		dst.mode.THREE.end_col.integer = src.items[2].col.ToRGBA();
		dst.mode.THREE.begin_pos = src.items[0].pos;
		dst.mode.THREE.mid_pos = src.items[1].pos;
		dst.mode.THREE.end_pos = src.items[2].pos;
		dst.mode.THREE.angle = src.angle;
	}
	else if (src.items.size() == 2)
	{
		dst.mode_type = 1;
		dst.mode.TWO.begin_col.integer = src.items[0].col.ToRGBA();
		dst.mode.TWO.end_col.integer = src.items[1].col.ToRGBA();
		dst.mode.TWO.begin_pos = src.items[0].pos;
		dst.mode.TWO.end_pos = src.items[1].pos;
		dst.mode.TWO.angle = src.angle;
	}
	else
	{
		assert(src.items.size() == 1);
		dst.mode_type = 0;
		dst.mode.ONE.color.integer = src.items[0].col.ToRGBA();
	}
}

sm::rect TextboxSymbol::GetBoundingImpl(const Sprite* spr, const Actor* actor, bool cache) const
{
	if (actor)
	{
		const sm::rect& r = actor->GetAABB().GetRect();
		auto tb_spr = S2_VI_DOWN_CAST<const TextboxSprite*>(spr);
		if (!r.IsValid()) {
			auto tb_actor = static_cast<const TextboxActor*>(actor);
			return TextboxActor::CalcAABB(tb_spr->GetTextbox(), GetBounding(), tb_actor->GetText());
		}

		sm::rect ret(r.Width(), r.Height());

		float offy = - (r.Height() - tb_spr->GetTextbox().height) * 0.5f;
		ret.ymin += offy;
		ret.ymax += offy;
		return ret;
	}
	else if (spr)
	{
		auto tb_spr = S2_VI_DOWN_CAST<const TextboxSprite*>(spr);
		return sm::rect(
			static_cast<float>(tb_spr->GetTextbox().width), 
			static_cast<float>(tb_spr->GetTextbox().height));
	}
	else 
	{
		return sm::rect(
			static_cast<float>(m_tb.width), 
			static_cast<float>(m_tb.height));
	}
}

pt2::RenderReturn TextboxSymbol::DrawImpl(cooking::DisplayList* dlist, const RenderParams& rp, const Sprite* spr) const
{
#ifndef S2_DISABLE_STATISTICS
	StatSymDraw::Instance()->AddDrawCount(STAT_SYM_TEXTBOX);
	//	StatSymDraw::DrawCostCP cp(STAT_SYM_TEXTBOX);
#endif // S2_DISABLE_STATISTICS

	const CU_STR* text = nullptr;
	if (rp.actor) {
		text = &S2_VI_DOWN_CAST<const TextboxActor*>(rp.actor)->GetText();
	}

	auto tb_spr = S2_VI_DOWN_CAST<const TextboxSprite*>(spr);

	if (!text) {
		text = &tb_spr->GetText(UpdateParams());
	}
	if (!text || text->empty()) {
		return pt2::RENDER_NO_DATA;
	}

	auto& shader_mgr = sl::Blackboard::Instance()->GetRenderContext().GetShaderMgr();
#ifdef S2_FILTER_FULL
	if (rp.render_filter && rp.render_filter->GetMode() == pt2::FM_GRAY) {
#else
	if (rp.render_filter == pt2::FM_GRAY) {
#endif // S2_FILTER_FULL
		shader_mgr.SetShader(sl::FILTER);
		sl::FilterShader* shader = static_cast<sl::FilterShader*>(shader_mgr.GetShader());
		shader->SetMode(sl::FM_GRAY);
	} else {
		shader_mgr.SetShader(sl::SPRITE2);
	}

	const pt2::Textbox& tb = tb_spr->GetTextbox();

	gtxt_label_style s;

	s.width = tb.width;
	s.height = tb.height;

	s.gs.font = tb.font_type;
	s.gs.font_size = tb.font_size;
	CopyColor(s.gs.font_color, tb.font_color);

	s.gs.edge = tb.has_edge;
	s.gs.edge_size = tb.edge_size;
	CopyColor(s.gs.edge_color, tb.edge_color);

	s.align_h = tb.align_hori;
	s.align_v = tb.align_vert;

	s.space_h = tb.space_hori;
	s.space_v = tb.space_vert;

	s.overflow = tb.overflow;

	DrawText(dlist, s, rp.mt, rp.col_common.mul, rp.col_common.add, *text, tb_spr->GetTime(), tb.richtext);

	tb_spr->UpdateTime();

	return pt2::RENDER_OK;
}

}