#include "sprite2/AudioContext.h"

namespace s2
{

static AudioContext::Callback CB;

void AudioContext::InitCallback(const Callback& cb)
{
	CB = cb;
}

bool AudioContext::IsEnable()
{
	return CB.is_enable();
}

}