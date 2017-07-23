#include "AudioSprite.h"
#include "AudioSymbol.h"
#ifndef S2_DISABLE_STATISTICS
#include "sprite2/StatSprCount.h"
#endif // S2_DISABLE_STATISTICS

#include <uniaudio/Source.h>

namespace s2
{

AudioSprite::AudioSprite()
{
#ifndef S2_DISABLE_STATISTICS
	StatSprCount::Instance()->Add(STAT_SYM_AUDIO);
#endif // S2_DISABLE_STATISTICS
}

AudioSprite::AudioSprite(const AudioSprite& spr)
	: Sprite(spr)
{
#ifndef S2_DISABLE_STATISTICS
	StatSprCount::Instance()->Add(STAT_SYM_AUDIO);
#endif // S2_DISABLE_STATISTICS
}

AudioSprite& AudioSprite::operator = (const AudioSprite& spr)
{
	Sprite::operator = (spr);
	
	return *this;
}

AudioSprite::AudioSprite(Symbol* sym, uint32_t id)
	: Sprite(sym, id)
{
#ifndef S2_DISABLE_STATISTICS
	StatSprCount::Instance()->Add(STAT_SYM_AUDIO);
#endif // S2_DISABLE_STATISTICS
}

AudioSprite::~AudioSprite()
{
#ifndef S2_DISABLE_STATISTICS
	StatSprCount::Instance()->Subtract(STAT_SYM_AUDIO);
#endif // S2_DISABLE_STATISTICS
}

AudioSprite* AudioSprite::Clone() const
{
	return new AudioSprite(*this);
}

void AudioSprite::Play()
{
	AudioSymbol* sym = VI_DOWNCASTING<AudioSymbol*>(m_sym);
	ua::Source* source = sym->GetSource();
	if (source) {
		source->Play();
	}
}

}