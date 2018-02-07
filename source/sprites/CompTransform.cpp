#include "sprite2/CompTransform.h"

namespace s2
{

CompTransform* CompTransform::Clone() const
{
	auto ret = static_cast<CompTransform*>(mm::AllocHelper::New<CompTransform>());
	memcpy(ret, this, sizeof(*this));
	return ret;
}

CompTransform* CompTransform::Create()
{
	auto ret = static_cast<CompTransform*>(mm::AllocHelper::New<CompTransform>());
	ret->m_transform.Reset();
	return ret;
}

}