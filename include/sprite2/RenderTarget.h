#ifndef _SPRITE2_RENDER_TARGET_H_
#define _SPRITE2_RENDER_TARGET_H_

namespace ur { class RenderTarget; }

namespace s2
{

class RenderTarget
{
public:
	RenderTarget(int width, int height);
	virtual ~RenderTarget();

	virtual void Bind();
	virtual void Unbind();

	int Width() const;
	int Height() const;

	int GetTexID() const;

	void Resize(int width, int height);

private:
	ur::RenderTarget* m_impl;	

}; // RenderTarget

}

#endif // _SPRITE2_RENDER_TARGET_H_