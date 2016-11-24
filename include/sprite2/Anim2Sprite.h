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
	Anim2Sprite(Anim2Symbol* sym, uint32_t id = -1);

	/**
	 *  @interface
	 *    Cloneable
	 */
	virtual Anim2Sprite* Clone() const;

	/**
	 *  @interface
	 *    Sprite
	 */
	virtual bool Update(const RenderParams& params);

	Anim2Curr& GetAnimCurr() { return m_curr; }

protected:
	Anim2Curr m_curr;

	VI_DUMMY_FUNC

}; // Anim2Sprite

}

#endif // _SPRITE2_ANIM2_SPRITE_H_