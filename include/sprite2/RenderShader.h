#ifndef _SPRITE2_RENDER_SHADER_H_
#define _SPRITE2_RENDER_SHADER_H_

#include "sprite2/config.h"
#include "sprite2/RenderFilter.h"

#include <painting2/BlendMode.h>
#include <painting2/FastBlendMode.h>
#include <painting2/FilterMode.h>

#include <memory>

#include <stddef.h>

namespace s2
{

class RenderShader
{
public:
	RenderShader();
	RenderShader(const RenderShader& rs);
	RenderShader& operator = (const RenderShader& rs);

	bool operator == (const RenderShader& rs) const;
	bool operator != (const RenderShader& rs) const;

	void Reset();

	RenderShader operator * (const RenderShader& rs) const;
#ifdef S2_FILTER_FULL
	RenderShader Multiply(const RenderFilter* filter,
		pt2::BlendMode blend, pt2::FastBlendMode fast_blend, float downsample) const;
#else
	RenderShader Multiply(pt2::FilterMode filter, pt2::BlendMode blend, pt2::FastBlendMode fast_blend, 
		float downsample) const;
#endif // S2_FILTER_FULL

	auto& GetFilter() const { return m_filter; }
	pt2::BlendMode GetBlend() const { return m_blend; }
	pt2::FastBlendMode GetFastBlend() const { return m_fast_blend; }
	float GetDownsample() const { return m_downsample; }

	void SetFilter(pt2::FilterMode mode);
#ifdef S2_FILTER_FULL
	void SetFilter(const RenderFilter* filter);
#endif // S2_FILTER_FULL
	void SetBlend(pt2::BlendMode mode) { m_blend = mode; }
	void SetFastBlend(pt2::FastBlendMode mode) { m_fast_blend = mode; }
	void SetDownsample(float downsample) { m_downsample = downsample; }

private:
#ifdef S2_FILTER_FULL
	RenderFilterPtr m_filter;
#else
	pt2::FilterMode      m_filter;
#endif // S2_FILTER_FULL
	pt2::BlendMode       m_blend;
	pt2::FastBlendMode   m_fast_blend;
	float                m_downsample;

}; // RenderShader

}

#endif // _SPRITE2_RENDER_SHADER_H_