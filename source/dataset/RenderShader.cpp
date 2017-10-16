#include "RenderShader.h"
#include "RenderFilter.h"
#include "FilterFactory.h"

namespace s2
{

RenderShader::RenderShader()
	: m_filter(nullptr)
	, m_blend(BM_NULL)
	, m_fast_blend(FBM_NULL)
	, m_downsample(1)
{
}

RenderShader::RenderShader(const RenderShader& rs)
{
	if (rs.m_filter) {
		m_filter = (rs.m_filter->Clone());
	} else {
		m_filter = nullptr;
	}
	m_blend      = rs.m_blend;
	m_fast_blend = rs.m_fast_blend;
	m_downsample = rs.m_downsample;
}

RenderShader& RenderShader::operator = (const RenderShader& rs)
{
	if (m_filter != rs.m_filter) 
	{
		if (m_filter) {
			delete m_filter;
		}
		if (rs.m_filter) {
			m_filter = rs.m_filter->Clone();
		} else {
			m_filter = nullptr;
		}
	}
	m_blend      = rs.m_blend;
	m_fast_blend = rs.m_fast_blend;
	m_downsample = rs.m_downsample;
	return *this;
}

bool RenderShader::operator == (const RenderShader& rs) const
{
	return
		(!m_filter && !rs.m_filter) || (m_filter && rs.m_filter && *m_filter == *rs.m_filter) &&
		m_blend == rs.m_blend &&
		m_fast_blend == rs.m_fast_blend &&
		m_downsample == rs.m_downsample;
}

bool RenderShader::operator != (const RenderShader& rs) const
{
	return !(this->operator == (rs));
}

void RenderShader::Reset()
{
	if (m_filter) {
		delete m_filter;
	}
	m_filter = nullptr;
	m_blend = BM_NULL;
	m_fast_blend = FBM_NULL;
	m_downsample = 1;
}

RenderShader RenderShader::operator * (const RenderShader& rs) const
{
	RenderShader ret;

	if (rs.m_blend != BM_NULL) {
		ret.m_blend = rs.m_blend;
	} else {
		ret.m_blend = m_blend;
	}

	if (rs.m_fast_blend != FBM_NULL) {
		ret.m_fast_blend = rs.m_fast_blend;
	} else {
		ret.m_fast_blend = m_fast_blend;
	}

	if (rs.m_filter && rs.m_filter->GetMode()!= FM_NULL) {
		ret.m_filter = rs.m_filter->Clone();
	} else if (m_filter) {
		ret.m_filter = m_filter->Clone();
	}

	ret.m_downsample = rs.m_downsample * m_downsample;

	return ret;
}

void RenderShader::SetFilter(FilterMode mode)
{
	if (m_filter && m_filter->GetMode() == mode) {
		return;
	}

	if (m_filter) {
		delete m_filter;
	}
	m_filter = FilterFactory::Instance()->Create(mode);
}

void RenderShader::SetFilter(const RenderFilter* filter)
{
	if (m_filter == filter) {
		return;
	}

	if (filter) {
		delete m_filter;
		m_filter = filter->Clone();
	} else {
		m_filter = nullptr;
	}
}

}