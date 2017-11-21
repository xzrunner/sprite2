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
	if (!AudioContext::IsEnable()) {
		return;
	}

	if (m_source) {
		m_source->Play();
	}
}

void AudioSprite::Stop()
{
	if (!AudioContext::IsEnable()) {
		return;
	}

	if (m_source) {
		m_source->Stop();
	}
}

void AudioSprite::Pause()
{
	if (!AudioContext::IsEnable()) {
		return;
	}

	if (m_source) {
		m_source->Pause();
	}
}

void AudioSprite::Resume()
{
	if (!AudioContext::IsEnable()) {
		return;
	}

	if (m_source) {
		m_source->Resume();
	}
}

void AudioSprite::SetVolume(float volume)
{
	m_volume = volume;

	if (m_source) {
		m_source->SetOriVolume(volume);
	}
}

void AudioSprite::SetFadeIn(float fade_in) 
{ 
	m_fade_in = fade_in;
	
	if (m_source) {
		m_source->SetFadeIn(fade_in);
	}
}

void AudioSprite::SetFadeOut(float fade_out) 
{ 
	m_fade_out = fade_out; 

	if (m_source) {
		m_source->SetFadeOut(fade_out);
	}
}

void AudioSprite::SetAudioOffset(float offset) 
{ 
	m_offset = offset; 

	if (m_source) {
		m_source->SetOffset(offset);
	}
}

void AudioSprite::SetAudioDuration(float duration) 
{ 
	m_duration = duration; 

	if (m_source) {
		m_source->SetDuration(duration);
	}
}

}