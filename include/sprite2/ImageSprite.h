#ifndef _SPRITE2_IMAGE_SPRITE_H_
#define _SPRITE2_IMAGE_SPRITE_H_

#include "S2_Sprite.h"

namespace s2
{

class ImageSprite : public VIRTUAL_INHERITANCE Sprite
{
public:
	ImageSprite();
	ImageSprite(Symbol* sym, uint32_t id = -1);

	/**
	 *  @interface
	 *    Cloneable
	 */
	virtual ImageSprite* Clone() const;

	/**
	 *  @interface
	 *    Sprite
	 */
	virtual bool Update(const RenderParams& params);	

	VI_DUMMY_FUNC

}; // ImageSprite

}

#endif // _SPRITE2_IMAGE_SPRITE_H_