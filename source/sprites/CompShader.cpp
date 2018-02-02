#include "sprite2/CompShader.h"

#include <memmgr/Allocator.h>

namespace s2
{

CompShader::~CompShader()
{
	mm::AllocHelper::Delete(this);
}

CompShader* CompShader::Clone() const
{
	auto ret = static_cast<CompShader*>(mm::AllocHelper::New<CompShader>());
	memcpy(ret, this, sizeof(*this));
	return ret;
}

CompShader* CompShader::Create()
{
	return static_cast<CompShader*>(mm::AllocHelper::New<CompShader>());
}

}