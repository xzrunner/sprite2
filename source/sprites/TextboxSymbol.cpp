#include "TextboxSymbol.h"
#include "SymType.h"
#include "TextboxSprite.h"
#include "RenderParams.h"
#include "RenderFilter.h"
#include "TextboxActor.h"
#include "DrawNode.h"

#include <gtxt_label.h>
#include <shaderlab/ShaderMgr.h>
#include <shaderlab/FilterMode.h>
#include <shaderlab/FilterShader.h>

namespace s2
{

TextboxSymbol::TextboxSymbol(uint32_t id)
	: Symbol(id)
{
}

TextboxSymbol::TextboxSymbol(Textbox tb)
	: m_tb(tb)
{
}

int TextboxSymbol::Type() const 
{ 
	return SYM_TEXTBOX; 
}

void TextboxSymbol::Draw(const RenderParams& rp, const Sprite* spr) const
{
	if (!spr) {
		return;
	}

	RenderParams rp_child;
	if (!DrawNode::Prepare(rp, spr, rp_child)) {
		return;
	}

	const std::string* text = NULL;
	const Actor* actor = rp.actor;
	if (actor) {
		const TextboxActor* tb_actor = static_cast<const TextboxActor*>(actor);
		text = &tb_actor->GetText();
	}
	
 	const TextboxSprite* tb_spr = VI_DOWNCASTING<const TextboxSprite*>(spr);

	if (!text) {
		text = &tb_spr->GetText();
	}
 	if (!text || text->empty()) {
 		return;
 	}

	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
	if (rp_child.shader.GetFilter() && sl::FILTER_MODE(rp_child.shader.GetFilter()->GetMode()) == sl::FM_GRAY) {
		mgr->SetShader(sl::FILTER);
		sl::FilterShader* shader = static_cast<sl::FilterShader*>(mgr->GetShader());
		shader->SetMode(sl::FM_GRAY);
	} else {
		mgr->SetShader(sl::SPRITE2);
	}

	const Textbox& tb = tb_spr->GetTextbox();
 
 	gtxt_label_style s;
 
 	s.width					= tb.width;
 	s.height				= tb.height;
 
 	s.gs.font				= tb.font_type;
 	s.gs.font_size			= tb.font_size;
 	s.gs.font_color.integer = tb.font_color.ToRGBA();
 
 	s.gs.edge				= tb.has_edge;
 	s.gs.edge_size			= tb.edge_size;
 	s.gs.edge_color.integer = tb.edge_color.ToRGBA();
 
 	s.align_h				= tb.align_hori;
 	s.align_v				= tb.align_vert;
 
 	s.space_h				= tb.space_hori;
 	s.space_v				= tb.space_vert;

	s.overflow				= tb.overflow;

	DrawText(s, rp_child.mt, rp_child.color.GetMul(), rp_child.color.GetAdd(), *text, tb_spr->GetTime(), tb.richtext);

	tb_spr->UpdateTime();
}

sm::rect TextboxSymbol::GetBounding(const Sprite* spr) const
{
	int w, h;
	if (spr) {
		const sm::vec2& sz = VI_DOWNCASTING<const TextboxSprite*>(spr)->GetSize();
		w = sz.x;
		h = sz.y;
	} else {
		w = m_tb.width;
		h = m_tb.height;
	}
	return sm::rect(w, h);
}

}