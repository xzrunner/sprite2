#ifndef _SPRITE2_ANIM2_SPRITE_H_
#define _SPRITE2_ANIM2_SPRITE_H_

#include "S2_Sprite.h"

namespace s2
{

class Anim2Sprite : public VIRTUAL_INHERITANCE Sprite
{
public:
	Anim2Sprite();
	Anim2Sprite(Symbol* sym);

	/**
	 *  @interface
	 *    Cloneable
	 */
	virtual Anim2Sprite* Clone() const;

protected:
	VI_DUMMY_FUNC

}; // Anim2Sprite

}

#endif // _SPRITE2_ANIM2_SPRITE_H_