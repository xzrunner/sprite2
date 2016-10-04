#include "TextboxSymbol.h"
#include "SymType.h"
#include "TextboxSprite.h"
#include "RenderParams.h"
#include "RenderFilter.h"

#include <gtxt_label.h>
#include <shaderlab.h>

namespace s2
{

TextboxSymbol::TextboxSymbol(Textbox tb)
	: m_tb(tb)
{
}

int TextboxSymbol::Type() const 
{ 
	return SYM_TEXTBOX; 
}

void TextboxSymbol::Draw(const RenderParams& params, const Sprite* spr) const
{
	if (!spr) {
		return;
	}

 	const TextboxSprite* tb_spr = VI_DOWNCASTING<const TextboxSprite*>(spr); 	
 	const std::string& text = tb_spr->GetText();
 	if (text.empty()) {
 		return;
 	}

	RenderParams p = params;
	if (spr) {
		p.mt = spr->GetTransMatrix() * p.mt;
		p.color = spr->Color() * p.color;
	}

	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
	if (p.shader.filter && p.shader.filter->GetMode() == sl::FM_GRAY) {
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

	DrawText(s, p.mt, p.color.mul, p.color.add, tb_spr->GetText(), tb_spr->GetTime(), tb.richtext);

	tb_spr->UpdateTime();
}

sm::rect TextboxSymbol::GetBounding(const Sprite* spr) const
{
	int w, h;
	if (spr) {
		const Textbox& tb = VI_DOWNCASTING<const TextboxSprite*>(spr)->GetTextbox();
		w = tb.width;
		h = tb.height;
	} else {
		w = m_tb.width;
		h = m_tb.height;
	}
	return sm::rect(w, h);
}

}