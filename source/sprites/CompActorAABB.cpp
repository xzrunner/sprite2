#include "sprite2/CompActorAABB.h"

#include <memmgr/Allocator.h>

namespace s2
{

CompActorAABB* CompActorAABB::Clone() const
{
	auto ret = static_cast<CompActorAABB*>(mm::AllocHelper::New<CompActorAABB>());
	memcpy(ret, this, sizeof(*this));
	return ret;
}

CompActorAABB* CompActorAABB::Create()
{
	return static_cast<CompActorAABB*>(mm::AllocHelper::New<CompActorAABB>());
}

}