#include "sprite2/SprRender.h"
#include "sprite2/RenderShader.h"
#include "sprite2/SprDefault.h"

#include <memmgr/Allocator.h>

namespace s2
{

SprRender::SprRender()
	: m_shader(SprDefault::Instance()->Shader())
{
}

SprRender::SprRender(RenderShader* shader)
	: m_shader(shader)
{
}

SprRender::~SprRender()
{
	if (m_shader != SprDefault::Instance()->Shader()) {
		mm::AllocHelper::Delete(m_shader);
	}
}

void SprRender::SetShader(const RenderShader& shader)
{
	if (*m_shader == shader) {
		return;
	}
	if (m_shader == SprDefault::Instance()->Shader()) {
		m_shader = mm::AllocHelper::New<RenderShader>(shader);
	} else {
		*m_shader = shader;
	}
}

}