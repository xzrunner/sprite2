#ifndef _SPRITE2_TEXTBOX_SPRITE_H_
#define _SPRITE2_TEXTBOX_SPRITE_H_

#include "sprite2/Sprite.h"

#include <painting2/Textbox.h>

namespace s2
{

class TextboxSprite : public VIRTUAL_INHERITANCE Sprite
{
public:
	TextboxSprite();
	TextboxSprite(const TextboxSprite& spr);
	TextboxSprite& operator = (const TextboxSprite& spr);
	TextboxSprite(const SymPtr& sym, uint32_t id = -1);
	virtual ~TextboxSprite();

	const pt2::Textbox& GetTextbox() const { return m_tb; }
	pt2::Textbox& GetTextbox() { return m_tb; }

	const CU_STR& GetText(const UpdateParams& up) const;
	void SetText(const UpdateParams& up, const CU_STR& text);

	const CU_STR& GetTID() const { return m_tid; }
	void SetTID(const CU_STR& tid) { m_tid = tid; }

	int GetTime() const { return m_time; }
	void UpdateTime() const { ++m_time; }
	void ResetTime() { m_time = 0; }

protected:
	pt2::Textbox m_tb;

	CU_STR m_text;
	CU_STR m_tid;

private:
	// for dynamic draw
	mutable int m_time;	

	S2_SPR_CLONE_FUNC(TextboxSprite)

	VI_DUMMY_FUNC

}; // TextboxSprite

}

#endif // _SPRITE2_TEXTBOX_SPRITE_H_