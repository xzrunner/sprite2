#ifndef _SPRITE2_ANIM_STATE_H_
#define _SPRITE2_ANIM_STATE_H_

#include "sprite2/config.h"
#include "sprite2/AnimCurr.h"
#include "sprite2/typedef.h"

#include <memory>

namespace s2
{

class AnimCopy;
class Sprite;

class AnimState
{
public:
	AnimState();

	void Assign(const AnimState& src, bool same_struct);

	void Init(const std::shared_ptr<AnimCopy>& copy);

	void Start(const Sprite* spr);

	AnimCurr& GetOrigin() { return *m_origin; }
#ifdef S2_MULTITHREAD
	AnimCurr& GetUpdate() { return *m_update; }
	AnimCurr& GetDraw()   { return *m_draw; }
#endif // S2_MULTITHREAD

	const AnimCurr& GetOrigin() const { return *m_origin; }
#ifdef S2_MULTITHREAD
	const AnimCurr& GetUpdate() const { return *m_update; }
	const AnimCurr& GetDraw() const { return *m_draw; }
#endif // S2_MULTITHREAD

	const AnimCurrPtr& GetOriginPtr() const { return m_origin; }

#ifdef S2_MULTITHREAD
	void Flush();
#endif // S2_MULTITHREAD

private:
	void Init(AnimCurrPtr& dst, const std::shared_ptr<AnimCopy>& copy);

private:
	AnimCurrPtr m_origin;
#ifdef S2_MULTITHREAD
	AnimCurrPtr m_update;
	AnimCurrPtr m_draw;
#endif // S2_MULTITHREAD

}; // AnimState

}

#endif // _SPRITE2_ANIM_STATE_H_