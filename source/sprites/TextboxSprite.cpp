#include "sprite2/TextboxSprite.h"
#include "sprite2/TextboxSymbol.h"
#include "sprite2/UpdateParams.h"
#include "sprite2/TextboxActor.h"
#ifndef S2_DISABLE_STATISTICS
#include "sprite2/StatSprCount.h"
#endif // S2_DISABLE_STATISTICS

#include <stddef.h>
#include <assert.h>

namespace s2
{

TextboxSprite::TextboxSprite()
	: m_time(0)
{
#ifndef S2_DISABLE_STATISTICS
	StatSprCount::Instance()->Add(STAT_SYM_TEXTBOX);
#endif // S2_DISABLE_STATISTICS
}

TextboxSprite::TextboxSprite(const TextboxSprite& spr)
	: Sprite(spr)
	, m_tb(spr.m_tb)
	, m_text(spr.m_text)
	, m_tid(spr.m_tid)
	, m_time(spr.m_time)
{
#ifndef S2_DISABLE_STATISTICS
	StatSprCount::Instance()->Add(STAT_SYM_TEXTBOX);
#endif // S2_DISABLE_STATISTICS
}

TextboxSprite& TextboxSprite::operator = (const TextboxSprite& spr)
{
	Sprite::operator = (spr);

	m_tb   = spr.m_tb;
	m_text = spr.m_text;
	m_tid  = spr.m_tid;
	m_time = spr.m_time;

	return *this;
}

TextboxSprite::TextboxSprite(const SymPtr& sym, uint32_t id)
	: Sprite(sym, id)
	, m_time(0)
{
#ifndef S2_DISABLE_STATISTICS
	StatSprCount::Instance()->Add(STAT_SYM_TEXTBOX);
#endif // S2_DISABLE_STATISTICS

	auto tb_sym = S2_VI_PTR_DOWN_CAST<TextboxSymbol>(sym);
	assert(tb_sym);
	m_tb = tb_sym->GetTextbox();
}

TextboxSprite::~TextboxSprite()
{
#ifndef S2_DISABLE_STATISTICS
	StatSprCount::Instance()->Subtract(STAT_SYM_TEXTBOX);
#endif // S2_DISABLE_STATISTICS	
}

const CU_STR& TextboxSprite::GetText(const UpdateParams& up) const
{ 
	auto actor = up.GetActor();
	if (actor) {
		return S2_VI_DOWN_CAST<const TextboxActor*>(actor)->GetText();
	} else {
		return m_text;
	}
}

void TextboxSprite::SetText(const UpdateParams& up, const CU_STR& text) 
{
	auto actor = up.GetActor();
	if (actor) {
		const_cast<TextboxActor*>(
			S2_VI_DOWN_CAST<const TextboxActor*>(actor))->SetText(text);
	} else {
		m_text = text;
	}
}

}