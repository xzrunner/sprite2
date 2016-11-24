#ifndef _SPRITE2_SHAPE_SPRITE_H_
#define _SPRITE2_SHAPE_SPRITE_H_

#include "S2_Sprite.h"

namespace s2
{

class ShapeSprite : public VIRTUAL_INHERITANCE Sprite
{
public:
	ShapeSprite();
	ShapeSprite(Symbol* sym, uint32_t id = -1);

	/**
	 *  @interface
	 *    Cloneable
	 */
	virtual ShapeSprite* Clone() const;

	VI_DUMMY_FUNC

}; // ShapeSprite

}

#endif // _SPRITE2_SHAPE_SPRITE_H_