#ifndef _SPRITE2_TEXTBOX_ACTOR_H_
#define _SPRITE2_TEXTBOX_ACTOR_H_

#include "S2_Actor.h"

#include <string>

namespace s2
{

class TextboxActor : public Actor
{
public:
	TextboxActor(Sprite* spr, const SprTreePath& path)
		: Actor(spr, path) {}

	void SetText(const std::string& text) { m_text = text; }
	const std::string& GetText() const { return m_text; }

private:
	std::string m_text;

}; // TextboxActor

}

#endif // _SPRITE2_TEXTBOX_ACTOR_H_