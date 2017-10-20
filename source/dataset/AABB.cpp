#include "sprite2/AABB.h"

#include <memmgr/Allocator.h>

namespace s2
{

BoundingBox* AABB::Clone() const 
{
	void* ptr = mm::AllocHelper::Allocate(Size());
	return new (ptr) AABB(*this);
}

}