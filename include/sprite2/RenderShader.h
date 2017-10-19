#ifndef _SPRITE2_RENDER_SHADER_H_
#define _SPRITE2_RENDER_SHADER_H_

#include "s2_config.h"
#include "BlendMode.h"
#include "FastBlendMode.h"
#include "FilterMode.h"
#include "RenderFilter.h"

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
		BlendMode blend, FastBlendMode fast_blend, float downsample) const;
#else
	RenderShader Multiply(FilterMode filter, BlendMode blend, FastBlendMode fast_blend, 
		float downsample) const;
#endif // S2_FILTER_FULL

	auto& GetFilter() const { return m_filter; }
	BlendMode GetBlend() const { return m_blend; }
	FastBlendMode GetFastBlend() const { return m_fast_blend; }
	float GetDownsample() const { return m_downsample; }

	void SetFilter(FilterMode mode);
#ifdef S2_FILTER_FULL
	void SetFilter(const RenderFilter* filter);
#endif // S2_FILTER_FULL
	void SetBlend(BlendMode mode) { m_blend = mode; }
	void SetFastBlend(FastBlendMode mode) { m_fast_blend = mode; }
	void SetDownsample(float downsample) { m_downsample = downsample; }

private:
#ifdef S2_FILTER_FULL
	RenderFilterPtr m_filter;
#else
	FilterMode      m_filter;
#endif // S2_FILTER_FULL
	BlendMode       m_blend;
	FastBlendMode   m_fast_blend;
	float           m_downsample;

}; // RenderShader

}

#endif // _SPRITE2_RENDER_SHADER_H_