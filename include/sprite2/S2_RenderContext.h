#ifndef _SPRITE2_RENDER_CONTEXT_H_
#define _SPRITE2_RENDER_CONTEXT_H_

#include <SM_Vector.h>

namespace s2
{

class RenderContext
{
public:
	RenderContext();
	RenderContext(float proj_width, float proj_height, int screen_width, int screen_height);

	void SetModelView(const sm::vec2& offset, float scale);
	void SetProjection(int width, int height);
	void SetScreen(int width, int height);
	void SetViewport(int x, int y, int w, int h);

	void UpdateMVP() const;
	void UpdateModelView() const;
	void UpdateProjection() const;
	void UpdateViewport() const;

	const sm::vec2& GetMVOffset() const { return m_mv_offset; }
	float GetMVScale() const { return m_mv_scale; }

	int  GetScreenWidth() const { return m_screen_width; }
	int  GetScreenHeight() const { return m_screen_height; }

private:
	sm::vec2 m_mv_offset;
	float    m_mv_scale;

	float m_proj_width;
	float m_proj_height;

	int m_screen_width;
	int m_screen_height;

	int m_vp_x, m_vp_y, m_vp_w, m_vp_h;

}; // RenderContext

}

#endif // _SPRITE2_RENDER_CONTEXT_H_