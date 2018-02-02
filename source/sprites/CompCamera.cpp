#include "sprite2/CompCamera.h"

#include <memmgr/Allocator.h>

namespace s2
{

CompCamera::~CompCamera()
{
	mm::AllocHelper::Delete(this);
}

CompCamera* CompCamera::Clone() const
{
	auto ret = static_cast<CompCamera*>(mm::AllocHelper::New<CompCamera>());
	memcpy(ret, this, sizeof(*this));
	return ret;
}

CompCamera* CompCamera::Create()
{
	return static_cast<CompCamera*>(mm::AllocHelper::New<CompCamera>());
}

}