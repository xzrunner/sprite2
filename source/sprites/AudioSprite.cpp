#include "sprite2/AudioSprite.h"
#include "sprite2/AudioSymbol.h"
#ifndef S2_DISABLE_STATISTICS
#include "sprite2/StatSprCount.h"
#endif // S2_DISABLE_STATISTICS
#include "sprite2/AudioContext.h"

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

AudioSprite::AudioSprite(const SymPtr& sym, uint32_t id)
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

void AudioSprite::OnMessage(const UpdateParams& up, Message msg)
{
	switch (msg)
	{
	case MSG_PLAY: 
		Play();
		break;
	case MSG_STOP:
		Stop();
		break;
	case MSG_TRIGGER:
		Play();
		break;
	}
}

void AudioSprite::Play()
{
	if (!AudioContext::IsEnable()) {
		return;
	}

	auto& source = S2_VI_PTR_DOWN_CAST<AudioSymbol>(GetSymbol())->GetSource();
	if (source) {
		source->Play();
	}
}

void AudioSprite::Stop()
{
	if (!AudioContext::IsEnable()) {
		return;
	}

	auto& source = S2_VI_PTR_DOWN_CAST<AudioSymbol>(GetSymbol())->GetSource();
	if (source) {
		source->Stop();
	}
}

void AudioSprite::Pause()
{
	if (!AudioContext::IsEnable()) {
		return;
	}

	auto& source = S2_VI_PTR_DOWN_CAST<AudioSymbol>(GetSymbol())->GetSource();
	if (source) {
		source->Pause();
	}
}

void AudioSprite::Resume()
{
	if (!AudioContext::IsEnable()) {
		return;
	}

	auto& source = S2_VI_PTR_DOWN_CAST<AudioSymbol>(GetSymbol())->GetSource();
	if (source) {
		source->Resume();
	}
}

}