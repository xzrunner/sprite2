#include "sprite2/AudioSymbol.h"
#include "sprite2/SymType.h"
#ifndef S2_DISABLE_STATISTICS
#include "sprite2/StatSymCount.h"
#endif // S2_DISABLE_STATISTICS

#include <uniaudio/Source.h>

namespace s2
{

AudioSymbol::AudioSymbol()
{
#ifndef S2_DISABLE_STATISTICS
	StatSymCount::Instance()->Add(STAT_SYM_AUDIO);
#endif // S2_DISABLE_STATISTICS
}

AudioSymbol::AudioSymbol(uint32_t id)
	: Symbol(id)
{
#ifndef S2_DISABLE_STATISTICS
	StatSymCount::Instance()->Add(STAT_SYM_AUDIO);
#endif // S2_DISABLE_STATISTICS
}

AudioSymbol::~AudioSymbol()
{
#ifndef S2_DISABLE_STATISTICS
	StatSymCount::Instance()->Subtract(STAT_SYM_AUDIO);
#endif // S2_DISABLE_STATISTICS
}

int AudioSymbol::Type() const
{
	return SYM_AUDIO;
}

sm::rect AudioSymbol::GetBoundingImpl(const Sprite* spr, const Actor* actor, bool cache) const
{
	return sm::rect(); // empty
}

}