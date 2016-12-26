#include "RenderTarget.h"

#include <unirender/RenderContext.h>
#include <unirender/RenderTarget.h>
#include <shaderlab/ShaderMgr.h>

namespace s2
{

SINGLETON_DEFINITION(RenderTarget);

RenderTarget::RenderTarget()
{
	ur::RenderContext* rc = sl::ShaderMgr::Instance()->GetContext();
	m_rt0 = new ur::RenderTarget(rc, 1024, 1024);
	m_rt1 = new ur::RenderTarget(rc, 1024, 1024);
}

}