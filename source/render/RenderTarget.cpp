#include "RenderTarget.h"

#include <unirender/RenderContext.h>
#include <unirender/RenderTarget.h>
#include <unirender/Texture.h>
#include <shaderlab/ShaderMgr.h>

namespace s2
{

RenderTarget::RenderTarget(int width, int height)
{
	ur::RenderContext* rc = sl::ShaderMgr::Instance()->GetContext();
	m_impl = new ur::RenderTarget(rc, width, height);
}

RenderTarget::~RenderTarget()
{
	delete m_impl;
}

void RenderTarget::Bind()
{
	m_impl->Bind();
}

void RenderTarget::Unbind()
{
	m_impl->Unbind();
}

int RenderTarget::Width() const
{
	return m_impl->GetTexture()->Width();
}

int RenderTarget::Height() const
{
	return m_impl->GetTexture()->Height();
}

int RenderTarget::GetTexID() const
{
	return m_impl->GetTexture()->ID();
}

}