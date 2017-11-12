#ifndef _SPRITE2_AUDIO_SPRITE_H_
#define _SPRITE2_AUDIO_SPRITE_H_

#include "sprite2/Sprite.h"
#include "sprite2/macro.h"

namespace s2
{

class AudioSprite : public VIRTUAL_INHERITANCE Sprite
{
public:
	AudioSprite();
	AudioSprite(const AudioSprite& spr);
	AudioSprite& operator = (const AudioSprite& spr);
	AudioSprite(const SymPtr& sym, uint32_t id = -1);
	virtual ~AudioSprite();

	/**
	 *  @interface
	 *    Sprite
	 */
	virtual void OnMessage(const UpdateParams& up, Message msg) override;

	void Play();
	void Stop();
	void Pause();
	void Resume();

	S2_SPR_CLONE_FUNC(AudioSprite)

	VI_DUMMY_FUNC

}; // AudioSprite

}

#endif // _SPRITE2_AUDIO_SPRITE_H_