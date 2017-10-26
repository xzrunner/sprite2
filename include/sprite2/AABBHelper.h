#ifndef _SPRITE2_AABB_HELPER_H_
#define _SPRITE2_AABB_HELPER_H_

#include "sprite2/typedef.h"

#include <SM_Rect.h>
#include <cu/cu_stl.h>

#include <memory>

namespace s2
{

class Sprite;
class Actor;

class AABBHelper
{
public:
	static sm::rect CalcAABB(const CU_VEC<SprPtr>& children, const Actor* parent);

private:
	static void UpdateChildrenAABBTight(const CU_VEC<SprPtr>& children,
		const Actor* parent, const sm::rect& parent_aabb);

}; // AABBHelper

}

#endif // _SPRITE2_AABB_HELPER_H_