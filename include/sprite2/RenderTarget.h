#ifndef _SPRITE2_RENDER_TARGET_H_
#define _SPRITE2_RENDER_TARGET_H_

#include <CU_Singleton.h>

namespace ur { class RenderTarget; }

namespace s2
{

class RenderTarget
{
public:
	ur::RenderTarget* GetRT0() { return m_rt0; }
	ur::RenderTarget* GetRT1() { return m_rt1; }

private:
	ur::RenderTarget *m_rt0, *m_rt1;

	SINGLETON_DECLARATION(RenderTarget);

}; // RenderTarget

}

#endif // _SPRITE2_RENDER_TARGET_H_