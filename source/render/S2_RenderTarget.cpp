#include "S2_RenderTarget.h"

#include <unirender/UR_RenderContext.h>
#include <unirender/UR_RenderTarget.h>
#include <unirender/UR_Texture.h>
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
	return m_impl->Width();
}

int RenderTarget::Height() const
{
	return m_impl->Height();
}

int RenderTarget::GetTexID() const
{
	return m_impl->TexID();
}

void RenderTarget::Resize(int width, int height)
{
	m_impl->Resize(width, height);
}

}