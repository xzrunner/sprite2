#ifndef _SPRITE2_IMAGE_SPRITE_H_
#define _SPRITE2_IMAGE_SPRITE_H_

#include "S2_Sprite.h"

namespace s2
{

class ImageSprite : public VIRTUAL_INHERITANCE Sprite
{
public:
	ImageSprite();
	ImageSprite(const ImageSprite& spr);
	ImageSprite& operator = (const ImageSprite& spr);
	ImageSprite(Symbol* sym, uint32_t id = -1);
	virtual ~ImageSprite();

	/**
	 *  @interface
	 *    Cloneable
	 */
	virtual ImageSprite* Clone() const;

	/**
	 *  @interface
	 *    Sprite
	 */
	virtual bool Update(const UpdateParams& up);
	virtual bool NeedAutoUpdate(const Actor* actor) const { return false; }	// todo
	virtual bool AutoUpdate(const Actor* actor);

	VI_DUMMY_FUNC

}; // ImageSprite

}

#endif // _SPRITE2_IMAGE_SPRITE_H_