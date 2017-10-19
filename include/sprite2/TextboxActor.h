#ifndef _SPRITE2_TEXTBOX_ACTOR_H_
#define _SPRITE2_TEXTBOX_ACTOR_H_

#include "S2_Actor.h"

#include <cu/cu_stl.h>

namespace s2
{

class Textbox;

class TextboxActor : public Actor
{
public:
	TextboxActor(const SprConstPtr& spr, const ActorConstPtr& parent) 
		: Actor(spr, parent) {}

	void SetText(const CU_STR& text);
	const auto& GetText() const { return m_text; }

	static sm::rect CalcAABB(const Textbox& tb, const sm::rect& rect, const CU_STR& text);
	
private:
	CU_STR m_text;

}; // TextboxActor

}

#endif // _SPRITE2_TEXTBOX_ACTOR_H_