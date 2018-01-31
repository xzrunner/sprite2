#include "sprite2/CompTransform.h"

namespace s2
{

CompTransform::CompTransform()
{
	Reset();
}

CompTransform::~CompTransform()
{
	mm::AllocHelper::Delete(this);
}

CompTransform* CompTransform::Clone() const
{
	auto ret = static_cast<CompTransform*>(mm::AllocHelper::New<CompTransform>());
	memcpy(ret, this, sizeof(*this));
	return ret;
}

void CompTransform::Reset()
{
	m_srt.Reset();
	m_mat.Identity();
}

CompTransform* CompTransform::Create()
{
	auto ret = static_cast<CompTransform*>(mm::AllocHelper::New<CompTransform>());
	ret->Reset();
	return ret;
}

}