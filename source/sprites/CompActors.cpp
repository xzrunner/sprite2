#include "sprite2/CompActors.h"

#include <memmgr/Allocator.h>

namespace s2
{

CompActors::~CompActors()
{
	mm::AllocHelper::Delete(this);
}

CompActors* CompActors::Clone() const
{
	// no copy
	auto ret = static_cast<CompActors*>(mm::AllocHelper::New<CompActors>());
	return ret;
}

CompActors* CompActors::Create()
{
	return static_cast<CompActors*>(mm::AllocHelper::New<CompActors>());
}

}