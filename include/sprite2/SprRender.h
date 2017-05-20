#ifndef _SPRITE2_SPR_RENDER_H_
#define _SPRITE2_SPR_RENDER_H_

#include "ObjectPool.h"

#include <CU_RefCountObj.h>

namespace s2
{

class RenderColor;
class RenderShader;
class RenderCamera;

class SprRender : public cu::RefCountObj
{
public:
	SprRender();
	SprRender(RenderColor* color, RenderShader* shader, 
		RenderCamera* camera, float downsample);
	~SprRender();

	const RenderColor*  GetColor() const { return m_state.color; }
	const RenderShader* GetShader() const { return m_state.shader; }
	const RenderCamera* GetCamera() const { return m_state.camera; }
	float GetDownsample() const { return m_state.downsample; }

	void SetColor(const RenderColor& color);
	void SetShader(const RenderShader& shader);
	void SetCamera(const RenderCamera& camera);
	void SetDownsample(float downsample);

	/**
	 *  @interface
	 *    ObjectPool
	 */
	void Init();
	void Term();
	SprRender* GetNext() const { return m_state.next; }
	void SetNext(SprRender* next) { m_state.next = next; }

private:
	union
	{
		struct
		{
			RenderColor*  color;
			RenderShader* shader;
			RenderCamera* camera;
			float downsample;
		};

		SprRender* next;

	} m_state;

}; // SprRender

typedef ObjectPool<SprRender> SprRenderPool;

}

#endif // _SPRITE2_SPR_RENDER_H_