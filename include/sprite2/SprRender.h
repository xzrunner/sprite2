#ifndef _SPRITE2_SPR_RENDER_H_
#define _SPRITE2_SPR_RENDER_H_

namespace s2
{

class RenderShader;
class RenderCamera;

class SprRender
{
public:
	SprRender();
	SprRender(RenderShader* shader, RenderCamera* camera);
	~SprRender();

	const auto& GetShader() const { return m_shader; }
	const auto& GetCamera() const { return m_camera; }

	void SetShader(const RenderShader& shader);
	void SetCamera(const RenderCamera& camera);

private:
	RenderShader* m_shader;
	RenderCamera* m_camera;

}; // SprRender

}

#endif // _SPRITE2_SPR_RENDER_H_