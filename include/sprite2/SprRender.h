#ifndef _SPRITE2_SPR_RENDER_H_
#define _SPRITE2_SPR_RENDER_H_

#include "ObjectPool.h"

namespace s2
{

class RenderColor;
class RenderShader;
class RenderCamera;

class SprRender
{
public:
	SprRender();
	SprRender(RenderColor* color, RenderShader* shader, 
		RenderCamera* camera);
	~SprRender();

	const RenderColor*  GetColor() const { return m_state.color; }
	const RenderShader* GetShader() const { return m_state.shader; }
	const RenderCamera* GetCamera() const { return m_state.camera; }

	void SetColor(const RenderColor& color);
	void SetShader(const RenderShader& shader);
	void SetCamera(const RenderCamera& camera);

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
		};

		SprRender* next;

	} m_state;

}; // SprRender

typedef ObjectPool<SprRender> SprRenderPool;

}

#endif // _SPRITE2_SPR_RENDER_H_