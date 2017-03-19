#ifndef _SPRITE2_MASK_SPRITE_H_
#define _SPRITE2_MASK_SPRITE_H_

#include "S2_Sprite.h"

namespace s2
{

class MaskSprite : public VIRTUAL_INHERITANCE Sprite
{
public:
	MaskSprite();
	MaskSprite(Symbol* sym, uint32_t id = -1);

	/**
	 *  @interface
	 *    Cloneable
	 */
	virtual MaskSprite* Clone() const;

	/**
	 *  @interface
	 *    Sprite
	 */
	virtual void OnMessage(Message msg, const SprTreePath& path);
	virtual bool Update(const RenderParams& rp);
	virtual bool SetFrame(int frame, const SprTreePath& parent_path, bool force = false);
	virtual Sprite* FetchChild(const std::string& name, const SprTreePath& path) const;

	VI_DUMMY_FUNC

}; // MaskSprite

}

#endif // _SPRITE2_MASK_SPRITE_H_