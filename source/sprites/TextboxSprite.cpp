#include "TextboxSprite.h"
#include "TextboxSymbol.h"
#include "UpdateParams.h"
#include "TextboxActor.h"

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

const std::string& TextboxSprite::GetText(const UpdateParams& up) const 
{ 
	const Actor* actor = up.GetActor();
	if (actor) {
		return static_cast<const TextboxActor*>(actor)->GetText();
	} else {
		return m_text;
	}
}

void TextboxSprite::SetText(const UpdateParams& up, const std::string& text) 
{
	Actor* actor = const_cast<Actor*>(up.GetActor());
	if (actor) {
		static_cast<TextboxActor*>(actor)->SetText(text);
	} else {
		m_text = text;
	}
}

}