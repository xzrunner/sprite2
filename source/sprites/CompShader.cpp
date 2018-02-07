#include "sprite2/CompShader.h"

#include <memmgr/Allocator.h>

namespace s2
{

CompShader* CompShader::Clone() const
{
	auto ret = static_cast<CompShader*>(mm::AllocHelper::New<CompShader>());
	ret->m_shader = m_shader;
	return ret;
}

CompShader* CompShader::Create()
{
	return static_cast<CompShader*>(mm::AllocHelper::New<CompShader>());
}

}