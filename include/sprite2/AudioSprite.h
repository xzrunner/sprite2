#ifndef _SPRITE2_AUDIO_SPRITE_H_
#define _SPRITE2_AUDIO_SPRITE_H_

#include "S2_Sprite.h"

namespace s2
{

class AudioSprite : public VIRTUAL_INHERITANCE Sprite
{
public:
	AudioSprite();
	AudioSprite(const AudioSprite& spr);
	AudioSprite& operator = (const AudioSprite& spr);
	AudioSprite(Symbol* sym, uint32_t id = -1);
	virtual ~AudioSprite();

	/**
	 *  @interface
	 *    Cloneable
	 */
	virtual AudioSprite* Clone() const;

	void Play();

private:

	VI_DUMMY_FUNC

}; // AudioSprite

}

#endif // _SPRITE2_AUDIO_SPRITE_H_