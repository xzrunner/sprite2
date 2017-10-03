#ifndef _SPRITE2_ACTOR_AABB_H_
#define _SPRITE2_ACTOR_AABB_H_

#include "s2_typedef.h"

#include <SM_Rect.h>

namespace s2
{

class Actor;

class ActorAABB
{
public:
	ActorAABB();

	void Init(const ActorConstPtr& curr);
	bool Update(const ActorConstPtr& curr);
	bool Combine(const ActorConstPtr& curr, const sm::rect& rect);

	void UpdateParent(const ActorConstPtr& curr);

	const sm::rect& GetRect() const { return m_rect; }
	void SetRect(const sm::rect& rect);

	void SetStaticRect(const sm::rect& rect);

private:
	sm::rect UpdateTight(const ActorConstPtr& curr);

	static bool IsRectTight(const sm::rect& inner, const sm::rect& outer);
	static sm::rect Build(const ActorConstPtr& curr);

private:
	sm::rect m_rect;

	bool m_static;

}; // ActorAABB

}

#endif // _SPRITE2_ACTOR_AABB_H_