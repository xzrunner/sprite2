#ifndef _SPRITE2_RENDER_SHADER_H_
#define _SPRITE2_RENDER_SHADER_H_

#include "BlendMode.h"
#include "FastBlendMode.h"
#include "FilterMode.h"
#include "ObjectPool.h"

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

	const RenderFilter* GetFilter() const { return m_state.filter; }
	BlendMode GetBlend() const { return m_state.blend; }
	FastBlendMode GetFastBlend() const { return m_state.fast_blend; }
	float GetDownsample() const { return m_state.downsample; }

	void SetFilter(FilterMode mode);
	void SetFilter(const RenderFilter* filter);
	void SetBlend(BlendMode mode) { m_state.blend = mode; }
	void SetFastBlend(FastBlendMode mode) { m_state.fast_blend = mode; }
	void SetDownsample(float downsample) { m_state.downsample = downsample; }

	/**
	 *  @interface
	 *    ObjectPool
	 */
	void Init();
	void Term();
	RenderShader* GetNext() const { return m_state.next; }
	void SetNext(RenderShader* next) { m_state.next = next; }

private:
	union
	{
		struct 
		{
			RenderFilter* filter;
			BlendMode     blend;
			FastBlendMode fast_blend;
			float         downsample;
		};

		RenderShader* next;

	} m_state;

}; // RenderShader

typedef ObjectPool<RenderShader> RenderShaderPool;

}

#endif // _SPRITE2_RENDER_SHADER_H_