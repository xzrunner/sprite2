#include "RenderShader.h"
#include "RenderFilter.h"
#include "FilterFactory.h"

namespace s2
{

RenderShader::RenderShader()
{
	Init();
}

RenderShader::RenderShader(const RenderShader& rs)
{
	m_state.filter     = NULL;
	m_state.blend      = rs.m_state.blend;
	m_state.fast_blend = rs.m_state.fast_blend;
	m_state.downsample = rs.m_state.downsample;

	if (rs.m_state.filter) {
		m_state.filter = rs.m_state.filter->Clone();
	}
}

RenderShader& RenderShader::operator = (const RenderShader& rs)
{
	if (m_state.filter != rs.m_state.filter)
	{
		if (m_state.filter) {
			delete m_state.filter;
			m_state.filter = NULL;
		}
		if (rs.m_state.filter) {
			m_state.filter = rs.m_state.filter->Clone();
		} else {
			m_state.filter = NULL;
		}
	}
	m_state.blend      = rs.m_state.blend;
	m_state.fast_blend = rs.m_state.fast_blend;
	m_state.downsample = rs.m_state.downsample;
	return *this;
}

RenderShader::~RenderShader()
{
	Term();
}

RenderShader RenderShader::operator * (const RenderShader& rs) const
{
	RenderShader ret;

	if (rs.m_state.blend != BM_NULL) {
		ret.m_state.blend = rs.m_state.blend;
	} else {
		ret.m_state.blend = m_state.blend;
	}

	if (rs.m_state.fast_blend != FBM_NULL) {
		ret.m_state.fast_blend = rs.m_state.fast_blend;
	} else {
		ret.m_state.fast_blend = m_state.fast_blend;
	}

	if (rs.m_state.filter && rs.m_state.filter->GetMode()!= FM_NULL) {
		ret.m_state.filter = rs.m_state.filter->Clone();
	} else if (m_state.filter) {
		ret.m_state.filter = m_state.filter->Clone();
	}

	ret.m_state.downsample = rs.m_state.downsample * m_state.downsample;

	return ret;
}

void RenderShader::SetFilter(FilterMode mode)
{
	if (m_state.filter && m_state.filter->GetMode() == mode) {
		return;
	}

	if (m_state.filter) {
		delete m_state.filter;
	}
	m_state.filter = FilterFactory::Instance()->Create(mode);
}

void RenderShader::SetFilter(const RenderFilter* filter)
{
	if (m_state.filter == filter) {
		return;
	}

	if (m_state.filter) {
		delete m_state.filter;
	}
	m_state.filter = filter->Clone();
}

void RenderShader::Init()
{
	m_state.filter     = NULL;
	m_state.blend      = BM_NULL;
	m_state.fast_blend = FBM_NULL;
	m_state.downsample = 1;
}

void RenderShader::Term()
{
	if (m_state.filter) {
		delete m_state.filter;
		m_state.filter = NULL;
	}
}

}