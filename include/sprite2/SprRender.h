#ifndef _SPRITE2_SPR_RENDER_H_
#define _SPRITE2_SPR_RENDER_H_

namespace s2
{

class RenderShader;

class SprRender
{
public:
	SprRender();
	SprRender(RenderShader* shader);
	~SprRender();

	const auto& GetShader() const { return m_shader; }

	void SetShader(const RenderShader& shader);

private:
	RenderShader* m_shader;

}; // SprRender

}

#endif // _SPRITE2_SPR_RENDER_H_