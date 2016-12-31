#include "RenderTarget.h"

#include <unirender/RenderContext.h>
#include <unirender/RenderTarget.h>
#include <unirender/Texture.h>
#include <shaderlab/ShaderMgr.h>

namespace s2
{

SINGLETON_DEFINITION(RenderTarget);

RenderTarget::RenderTarget()
{
	ur::RenderContext* rc = sl::ShaderMgr::Instance()->GetContext();
	for (int i = 0; i < MAX_COUNT; ++i) {
		m_items[i].rt = new ur::RenderTarget(rc, WIDTH, HEIGHT);
		m_items[i].available = true;
	}
}

int RenderTarget::Fetch()
{
	for (int i = 0; i < MAX_COUNT; ++i) {
		if (m_items[i].available) {
			m_items[i].available = false;
			return i;
		}
	}
	return -1;
}

void RenderTarget::Return(int idx)
{
	if (idx < 0 || idx >= MAX_COUNT) {
		return;
	}
	m_items[idx].available = true;
}

void RenderTarget::Bind(int idx)
{
	if (idx < 0 || idx >= MAX_COUNT) {
		return;
	}
	m_items[idx].rt->Bind();	
}

void RenderTarget::Unbind(int idx)
{
	if (idx < 0 || idx >= MAX_COUNT) {
		return;
	}
	m_items[idx].rt->Unbind();
}

int RenderTarget::GetTexID(int idx)
{
	if (idx < 0 || idx >= MAX_COUNT) {
		return -1;
	}
	return m_items[idx].rt->GetTexture()->ID();
}

}