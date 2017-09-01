#ifndef _SPRITE2_AUDIO_CONTEXT_H_
#define _SPRITE2_AUDIO_CONTEXT_H_

namespace s2
{

class AudioContext
{
public:
	struct Callback
	{
		bool (*is_enable)();
	};

	static void InitCallback(const Callback& cb);

	static bool IsEnable();

}; // AudioContext

}

#endif // _SPRITE2_AUDIO_CONTEXT_H_