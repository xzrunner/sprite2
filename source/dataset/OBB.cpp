#include "sprite2/OBB.h"

#include <memmgr/Allocator.h>

namespace s2
{

BoundingBox* OBB::Clone() const
{
	void* ptr = mm::AllocHelper::Allocate(Size());
	return new (ptr) OBB(*this);
}

}