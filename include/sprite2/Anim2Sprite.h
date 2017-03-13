#ifndef _SPRITE2_ANIM2_SPRITE_H_
#define _SPRITE2_ANIM2_SPRITE_H_

#include "S2_Sprite.h"
#include "Anim2Curr.h"

namespace s2
{

class Anim2Sprite : public VIRTUAL_INHERITANCE Sprite
{
public:
	Anim2Sprite();
	Anim2Sprite(Symbol* sym, uint32_t id = -1);

	/**
	 *  @interface
	 *    Cloneable
	 */
	virtual Anim2Sprite* Clone() const;

	/**
	 *  @interface
	 *    Sprite
	 */
	virtual bool Update(const RenderParams& rp);
	virtual bool SetFrame(int frame, const SprTreePath& parent_path);

	Anim2Curr& GetAnimCurr() { return m_curr; }

	int GetStaticTime() const { return m_static_time; }
	void SetStaticTime(int static_time);

protected:
	int m_static_time;

	Anim2Curr m_curr;

	VI_DUMMY_FUNC

}; // Anim2Sprite

}

#endif // _SPRITE2_ANIM2_SPRITE_H_