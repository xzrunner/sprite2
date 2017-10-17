#include "RenderShader.h"
#include "RenderFilter.h"
#include "FilterFactory.h"

namespace s2
{

RenderShader::RenderShader()
	: m_filter(nullptr, RenderFilter::Deleter)
	, m_blend(BM_NULL)
	, m_fast_blend(FBM_NULL)
	, m_downsample(1)
{
}

RenderShader::RenderShader(const RenderShader& rs)
	: m_filter(nullptr, RenderFilter::Deleter)
{
	if (rs.m_filter) {
		m_filter.reset(rs.m_filter->Clone());
	}
	m_blend      = rs.m_blend;
	m_fast_blend = rs.m_fast_blend;
	m_downsample = rs.m_downsample;
}

RenderShader& RenderShader::operator = (const RenderShader& rs)
{
	if (m_filter != rs.m_filter) 
	{
		if (rs.m_filter) {
			m_filter.reset(rs.m_filter->Clone());
		} else {
			m_filter.reset();
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
	m_filter.reset();
	m_blend = BM_NULL;
	m_fast_blend = FBM_NULL;
	m_downsample = 1;
}

RenderShader RenderShader::operator * (const RenderShader& rs) const
{
	return Multiply(rs.GetFilter().get(), rs.GetBlend(), rs.GetFastBlend(), rs.GetDownsample());
}

RenderShader RenderShader::Multiply(const RenderFilter* filter,
	                                BlendMode blend, 
	                                FastBlendMode fast_blend, 
	                                float downsample) const
{
	RenderShader ret;

	if (blend != BM_NULL) {
		ret.m_blend = blend;
	}
	else {
		ret.m_blend = m_blend;
	}

	if (fast_blend != FBM_NULL) {
		ret.m_fast_blend = fast_blend;
	}
	else {
		ret.m_fast_blend = m_fast_blend;
	}

	if (filter && filter->GetMode() != FM_NULL) {
		ret.m_filter.reset(filter->Clone());
	}
	else if (m_filter) {
		ret.m_filter.reset(m_filter->Clone());
	}

	ret.m_downsample = downsample * m_downsample;

	return ret;
}

void RenderShader::SetFilter(FilterMode mode)
{
	if (m_filter && m_filter->GetMode() == mode) {
		return;
	}

	m_filter = FilterFactory::Instance()->Create(mode);
}

void RenderShader::SetFilter(const RenderFilter* filter)
{
	if (m_filter.get() == filter) {
		return;
	}

	if (filter) {
		m_filter.reset(filter->Clone());
	} else {
		m_filter.reset();
	}
}

}