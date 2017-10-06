#ifndef _SPRITE2_AABB_HELPER_H_
#define _SPRITE2_AABB_HELPER_H_

#include "s2_typedef.h"

#include <SM_Rect.h>

#include <vector>
#include <memory>

namespace s2
{

class Sprite;
class Actor;

class AABBHelper
{
public:
	static sm::rect CalcAABB(const std::vector<SprPtr>& children, const Actor* parent);

private:
	static void UpdateChildrenAABBTight(const std::vector<SprPtr>& children, 
		const Actor* parent, const sm::rect& parent_aabb);

}; // AABBHelper

}

#endif // _SPRITE2_AABB_HELPER_H_