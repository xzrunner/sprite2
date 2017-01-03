#ifndef _SPRITE2_RENDER_TARGET_H_
#define _SPRITE2_RENDER_TARGET_H_

namespace ur { class RenderTarget; }

namespace s2
{

class RenderTarget
{
public:
	RenderTarget(int width, int height);
	~RenderTarget();

	void Bind();
	void Unbind();

	int Width() const;
	int Height() const;

	int GetTexID() const;

private:
	ur::RenderTarget* m_impl;	

}; // RenderTarget

}

#endif // _SPRITE2_RENDER_TARGET_H_