#ifndef _SPRITE2_IMAGE_SPRITE_H_
#define _SPRITE2_IMAGE_SPRITE_H_

#include "sprite2/Sprite.h"

namespace s2
{

class ImageSprite : public VIRTUAL_INHERITANCE Sprite
{
public:
	ImageSprite();
	ImageSprite(const ImageSprite& spr);
	ImageSprite& operator = (const ImageSprite& spr);
	ImageSprite(const SymPtr& sym, uint32_t id = -1);
	virtual ~ImageSprite();

	/**
	 *  @interface
	 *    Sprite
	 */
	virtual bool Update(const UpdateParams& up) override;
	virtual bool NeedAutoUpdate(const Actor* actor) const override { return false; }	// todo
	virtual bool AutoUpdate(const Actor* actor) override;

	S2_SPR_CLONE_FUNC(ImageSprite)

	VI_DUMMY_FUNC

}; // ImageSprite

}

#endif // _SPRITE2_IMAGE_SPRITE_H_