#ifndef _SPRITE2_ANIM2_SPRITE_H_
#define _SPRITE2_ANIM2_SPRITE_H_

#include "sprite2/Sprite.h"
#include "sprite2/Anim2Curr.h"

namespace s2
{

class Anim2Sprite : public VIRTUAL_INHERITANCE Sprite
{
public:
	Anim2Sprite();
	Anim2Sprite(const Anim2Sprite& spr);
	Anim2Sprite& operator = (const Anim2Sprite& spr);
	Anim2Sprite(const SymPtr& sym, uint32_t id = -1);
	virtual ~Anim2Sprite();

	/**
	 *  @interface
	 *    Sprite
	 */
	virtual bool Update(const UpdateParams& up);
	virtual bool NeedAutoUpdate(const Actor* actor) const { return true; }
	virtual bool AutoUpdate(const Actor* actor);

	Anim2Curr& GetAnimCurr() { return m_curr; }

	int GetStaticTime() const { return m_static_time; }
	void SetStaticTime(const UpdateParams& up, int static_time);

protected:
	int m_static_time;

	Anim2Curr m_curr;

	S2_SPR_CLONE_FUNC(Anim2Sprite)

	VI_DUMMY_FUNC

}; // Anim2Sprite

}

#endif // _SPRITE2_ANIM2_SPRITE_H_