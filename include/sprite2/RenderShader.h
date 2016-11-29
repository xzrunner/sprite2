#ifndef _SPRITE2_RENDER_SHADER_H_
#define _SPRITE2_RENDER_SHADER_H_

#include "BlendMode.h"
#include "FastBlendMode.h"
#include "FilterMode.h"

#include <stddef.h>

namespace s2
{

class RenderFilter;

class RenderShader
{
public:
	RenderShader();
	RenderShader(const RenderShader& rs);
	RenderShader& operator = (const RenderShader& rs);
	~RenderShader();

	RenderShader operator * (const RenderShader& rs) const;

	const RenderFilter* GetFilter() const { return m_filter; }
	BlendMode GetBlend() const { return m_blend; }
	FastBlendMode GetFastBlend() const { return m_fast_blend; }

	void SetFilter(FilterMode mode);
	void SetFilter(const RenderFilter* filter);
	void SetBlend(BlendMode mode) { m_blend = mode; }
	void SetFastBlend(FastBlendMode mode) { m_fast_blend = mode; }

private:
	RenderFilter* m_filter;
	BlendMode     m_blend;
	FastBlendMode m_fast_blend;

}; // RenderShader

}

#endif // _SPRITE2_RENDER_SHADER_H_