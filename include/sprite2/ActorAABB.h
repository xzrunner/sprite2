#ifndef _SPRITE2_ACTOR_AABB_H_
#define _SPRITE2_ACTOR_AABB_H_

#include <SM_Rect.h>

namespace s2
{

class Actor;

class ActorAABB
{
public:
	ActorAABB() {}

	void Init(const Actor* curr);
	void Update(const Actor* curr);
	void Combine(const Actor* curr, const sm::rect& rect);

	void UpdateParent(const Actor* curr);

	const sm::rect& GetRect() const { return m_rect; }
	void SetRect(const sm::rect& rect) { m_rect = rect; }

private:
	void UpdateTight(const Actor* curr);

	static bool IsRectTight(const sm::rect& inner, const sm::rect& outer);
	static sm::rect Build(const Actor* curr);

private:
	sm::rect m_rect;

}; // ActorAABB

}

#endif // _SPRITE2_ACTOR_AABB_H_