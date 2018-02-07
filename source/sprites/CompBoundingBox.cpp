#include "sprite2/CompBoundingBox.h"

namespace s2
{

CompBoundingBox* CompBoundingBox::Clone() const
{
	auto ret = static_cast<CompBoundingBox*>(mm::AllocHelper::New<CompBoundingBox>());
	memcpy(ret, this, sizeof(*this));
	return ret;
}

CompBoundingBox* CompBoundingBox::Create()
{
	return static_cast<CompBoundingBox*>(mm::AllocHelper::New<CompBoundingBox>());
}

}