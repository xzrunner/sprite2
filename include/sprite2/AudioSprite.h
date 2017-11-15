#ifndef _SPRITE2_AUDIO_SPRITE_H_
#define _SPRITE2_AUDIO_SPRITE_H_

#include "sprite2/Sprite.h"
#include "sprite2/macro.h"

namespace ua { class Source; }

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

	void SetSource(const std::shared_ptr<ua::Source>& src) { m_source = src; }
	const std::shared_ptr<ua::Source>& GetSource() const { return m_source; }

	void Play();
	void Stop();
	void Pause();
	void Resume();

	float GetFadeIn() const { return m_fade_in; }
	void  SetFadeIn(float fade_in);
	float GetFadeOut() const { return m_fade_out; }
	void  SetFadeOut(float fade_out);

	float GetAudioOffset() const { return m_offset; }
	void  SetAudioOffset(float offset);
	float GetAudioDuration() const { return m_duration; }
	void  SetAudioDuration(float duration);

private:
	std::shared_ptr<ua::Source> m_source = nullptr;

	float m_offset, m_duration;
	float m_fade_in, m_fade_out;

	S2_SPR_CLONE_FUNC(AudioSprite)

	VI_DUMMY_FUNC

}; // AudioSprite

}

#endif // _SPRITE2_AUDIO_SPRITE_H_