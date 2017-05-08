#ifndef _SPRITE2_AABB_HELPER_H_
#define _SPRITE2_AABB_HELPER_H_

#include <SM_Rect.h>

#include <vector>

namespace s2
{

class Sprite;
class Actor;

class AABBHelper
{
public:
	static sm::rect CalcAABB(const std::vector<Sprite*>& children, const Actor* parent);

private:
	static void UpdateChildrenAABBTight(const std::vector<Sprite*>& children, const Actor* parent, const sm::rect& parent_aabb);

}; // AABBHelper

}

#endif // _SPRITE2_AABB_HELPER_H_