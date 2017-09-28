#ifndef _SPRITE2_ANIM_STATE_H_
#define _SPRITE2_ANIM_STATE_H_

#include "sprite2/s2_config.h"

#include <memory>

namespace s2
{

class AnimCurr;
class AnimCopy;
class Sprite;

class AnimState
{
public:
	void Assign(const AnimState& src, bool same_struct, const Sprite* spr);

	void Init(const std::shared_ptr<AnimCopy>& copy, const Sprite* spr);

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

#ifdef S2_MULTITHREAD
	void Flush();
#endif // S2_MULTITHREAD

private:
	void Init(std::unique_ptr<AnimCurr>& dst, 
		const std::shared_ptr<AnimCopy>& copy, const Sprite* spr);

	bool IsVaild() const {
#ifdef S2_MULTITHREAD
		return m_origin && m_update && m_draw;
#else
		return m_origin != nullptr;
#endif // S2_MULTITHREAD
	}

private:
	std::unique_ptr<AnimCurr> m_origin;
#ifdef S2_MULTITHREAD
	std::unique_ptr<AnimCurr> m_update;
	std::unique_ptr<AnimCurr> m_draw;
#endif // S2_MULTITHREAD

}; // AnimState

}

#endif // _SPRITE2_ANIM_STATE_H_