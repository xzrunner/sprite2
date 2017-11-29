#include "sprite2/AudioSprite.h"
#include "sprite2/AudioSymbol.h"
#ifndef S2_DISABLE_STATISTICS
#include "sprite2/StatSprCount.h"
#endif // S2_DISABLE_STATISTICS
#include "sprite2/AudioContext.h"

#include <uniaudio/Source.h>
#include <uniaudio/Exception.h>
#include <logger.h>

namespace s2
{

AudioSprite::AudioSprite()
	: m_volume(1)
	, m_offset(0)
	, m_duration(0)
	, m_fade_in(0)
	, m_fade_out(0)
{
#ifndef S2_DISABLE_STATISTICS
	StatSprCount::Instance()->Add(STAT_SYM_AUDIO);
#endif // S2_DISABLE_STATISTICS
}

AudioSprite::AudioSprite(const AudioSprite& spr)
	: Sprite(spr)
	, m_volume(spr.m_volume)
	, m_offset(spr.m_offset)
	, m_duration(spr.m_duration)
	, m_fade_in(spr.m_fade_in)
	, m_fade_out(spr.m_fade_out)
{
#ifndef S2_DISABLE_STATISTICS
	StatSprCount::Instance()->Add(STAT_SYM_AUDIO);
#endif // S2_DISABLE_STATISTICS

	if (spr.m_source) {
		m_source = spr.m_source->Clone();
	}
}

AudioSprite& AudioSprite::operator = (const AudioSprite& spr)
{
	Sprite::operator = (spr);

	if (spr.m_source) {
		m_source = spr.m_source->Clone();
	}

	m_volume   = spr.m_volume;

	m_offset   = spr.m_offset;
	m_duration = spr.m_duration;

	m_fade_in  = spr.m_fade_in;
	m_fade_out = spr.m_fade_out;
	
	return *this;
}

AudioSprite::AudioSprite(const SymPtr& sym, uint32_t id)
	: Sprite(sym, id)
	, m_volume(1)
	, m_offset(0)
	, m_duration(0)
	, m_fade_in(0)
	, m_fade_out(0)
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

void AudioSprite::SetSource(const std::shared_ptr<ua::Source>& src)
{
	m_source = src;

	m_volume = src->GetOriVolume();

	m_offset   = src->GetOffset();
	m_duration = src->GetDuration();

	m_fade_in  = src->GetFadeIn();
	m_fade_out = src->GetFadeOut();
}

void AudioSprite::Play()
{
	if (!AudioContext::IsEnable() || !m_source) {
		return;
	}

	try {
		m_source->Play();
	} catch (ua::Exception& e) {
		LOGW("audio play err: %s\n", e.what());
	}
}

void AudioSprite::Stop()
{
	if (!AudioContext::IsEnable() || !m_source) {
		return;
	}

	try {
		m_source->Stop();
	} catch (ua::Exception& e) {
		LOGW("audio stop err: %s\n", e.what());
	}
}

void AudioSprite::Pause()
{
	if (!AudioContext::IsEnable() || !m_source) {
		return;
	}

	try {
		m_source->Pause();
	} catch (ua::Exception& e) {
		LOGW("audio pause err: %s\n", e.what());
	}
}

void AudioSprite::Resume()
{
	if (!AudioContext::IsEnable() || !m_source) {
		return;
	}

	try {
		m_source->Resume();
	} catch (ua::Exception& e) {
		LOGW("audio resume err: %s\n", e.what());
	}
}

void AudioSprite::Seek(float offset)
{
	if (!AudioContext::IsEnable() || !m_source) {
		return;
	}

	try {
		m_source->Seek(offset);
	} catch (ua::Exception& e) {
		LOGW("audio seek err: %s\n", e.what());
	}
}

void AudioSprite::SetVolume(float volume)
{
	if (!m_source) {
		return;
	}

	m_volume = volume;

	try {
		m_source->SetOriVolume(volume);
	} catch (ua::Exception& e) {
		LOGW("audio set volume err: %s\n", e.what());
	}
}

void AudioSprite::SetFadeIn(float fade_in) 
{ 
	if (!m_source) {
		return;
	}

	m_fade_in = fade_in;

	try {
		m_source->SetFadeIn(fade_in);
	} catch (ua::Exception& e) {
		LOGW("audio set fade in err: %s\n", e.what());
	}
}

void AudioSprite::SetFadeOut(float fade_out)
{
	if (!m_source) {
		return;
	}

	m_fade_out = fade_out;

	try {
		m_source->SetFadeOut(fade_out);
	} catch (ua::Exception& e) {
		LOGW("audio set fade out err: %s\n", e.what());
	}
}

void AudioSprite::SetAudioOffset(float offset)
{
	if (!m_source) {
		return;
	}

	m_offset = offset;

	try {
		m_source->SetOffset(offset);
	} catch (ua::Exception& e) {
		LOGW("audio set offset err: %s\n", e.what());
	}
}

void AudioSprite::SetAudioDuration(float duration)
{
	if (!m_source) {
		return;
	}

	m_duration = duration;

	try {
		m_source->SetDuration(duration);
	} catch (ua::Exception& e) {
		LOGW("audio set duration err: %s\n", e.what());
	}
}

}