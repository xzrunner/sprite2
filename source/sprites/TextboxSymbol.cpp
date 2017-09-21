#include "TextboxSymbol.h"
#include "SymType.h"
#include "TextboxSprite.h"
#include "RenderParams.h"
#include "RenderFilter.h"
#include "TextboxActor.h"
#include "DrawNode.h"
#include "UpdateParams.h"
#include "FlattenParams.h"
#include "sprite2/Flatten.h"
#include "TextboxSprite.h"
#ifndef S2_DISABLE_STATISTICS
#include "sprite2/StatSymDraw.h"
#include "sprite2/StatSymCount.h"
#endif // S2_DISABLE_STATISTICS

#include <gtxt_label.h>
#include <shaderlab/ShaderMgr.h>
#include <shaderlab/FilterMode.h>
#include <shaderlab/FilterShader.h>

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

TextboxSymbol::TextboxSymbol(Textbox tb)
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

RenderReturn TextboxSymbol::Draw(const RenderParams& rp, const Sprite* spr) const
{
	return DrawImpl(nullptr, rp, spr);
}

RenderReturn TextboxSymbol::DrawDeferred(cooking::DisplayList* dlist, const RenderParams& rp, const Sprite* spr) const
{
	// todo
	return RENDER_OK;
}

bool TextboxSymbol::DrawFlatten(cooking::DisplayList* dlist, const RenderParams& rp, const Sprite* spr) const
{
	DrawImpl(dlist, rp, spr);
	return true;
}

void TextboxSymbol::Flattening(const FlattenParams& fp, Flatten& ft) const
{
	ft.AddNode(const_cast<Sprite*>(fp.GetSpr()), const_cast<Actor*>(fp.GetActor()), fp.GetPrevMat());
}

sm::rect TextboxSymbol::GetBoundingImpl(const Sprite* spr, const Actor* actor, bool cache) const
{
	if (actor)
	{
		const sm::rect& r = actor->GetAABB().GetRect();
		const TextboxSprite* tb_spr = VI_DOWNCASTING<const TextboxSprite*>(spr);
		if (!r.IsValid()) {
			const TextboxActor* tb_actor = VI_DOWNCASTING<const TextboxActor*>(actor);
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
		const TextboxSprite* tb_spr = VI_DOWNCASTING<const TextboxSprite*>(spr);
		return sm::rect(tb_spr->GetTextbox().width, tb_spr->GetTextbox().height);
	}
	else 
	{
		return sm::rect(m_tb.width, m_tb.height);
	}
}

RenderReturn TextboxSymbol::DrawImpl(cooking::DisplayList* dlist, const RenderParams& rp, const Sprite* spr) const
{
#ifndef S2_DISABLE_STATISTICS
	StatSymDraw::Instance()->AddDrawCount(STAT_SYM_TEXTBOX);
	//	StatSymDraw::DrawCostCP cp(STAT_SYM_TEXTBOX);
#endif // S2_DISABLE_STATISTICS

	if (!spr) {
		return RENDER_NO_DATA;
	}

	RenderParams* rp_child = RenderParamsPool::Instance()->Pop();
	*rp_child = rp;
	if (!DrawNode::Prepare(rp, spr, *rp_child)) {
		RenderParamsPool::Instance()->Push(rp_child);
		return RENDER_INVISIBLE;
	}

	const std::string* text = NULL;
	const Actor* actor = rp.actor;
	if (actor) {
		const TextboxActor* tb_actor = static_cast<const TextboxActor*>(actor);
		text = &tb_actor->GetText();
	}

	const TextboxSprite* tb_spr = VI_DOWNCASTING<const TextboxSprite*>(spr);

	if (!text) {
		text = &tb_spr->GetText(UpdateParams());
	}
	if (!text || text->empty()) {
		RenderParamsPool::Instance()->Push(rp_child);
		return RENDER_NO_DATA;
	}

	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
	if (rp_child->shader.GetFilter() && sl::FILTER_MODE(rp_child->shader.GetFilter()->GetMode()) == sl::FM_GRAY) {
		mgr->SetShader(sl::FILTER);
		sl::FilterShader* shader = static_cast<sl::FilterShader*>(mgr->GetShader());
		shader->SetMode(sl::FM_GRAY);
	}
	else {
		mgr->SetShader(sl::SPRITE2);
	}

	const Textbox& tb = tb_spr->GetTextbox();

	gtxt_label_style s;

	s.width = tb.width;
	s.height = tb.height;

	s.gs.font = tb.font_type;
	s.gs.font_size = tb.font_size;
	s.gs.font_color.integer = tb.font_color.ToRGBA();

	s.gs.edge = tb.has_edge;
	s.gs.edge_size = tb.edge_size;
	s.gs.edge_color.integer = tb.edge_color.ToRGBA();

	s.align_h = tb.align_hori;
	s.align_v = tb.align_vert;

	s.space_h = tb.space_hori;
	s.space_v = tb.space_vert;

	s.overflow = tb.overflow;

	DrawText(dlist, s, rp_child->mt, rp_child->color.GetMul(), rp_child->color.GetAdd(), *text, tb_spr->GetTime(), tb.richtext);

	tb_spr->UpdateTime();

	RenderParamsPool::Instance()->Push(rp_child);

	return RENDER_OK;
}

}