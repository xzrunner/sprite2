#include "sprite2/BoundingBox.h"

#include <memmgr/Allocator.h>

namespace s2
{

void BoundingBox::Deleter(BoundingBox* bb)
{
	size_t sz = bb->Size();
	bb->~BoundingBox();
	mm::AllocHelper::Free(bb, sz);
};

}