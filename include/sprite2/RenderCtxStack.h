#ifndef _SPRITE2_RENDER_CONTEXT_STACK_H_
#define _SPRITE2_RENDER_CONTEXT_STACK_H_

#include "sprite2/RenderContext.h"

#include <cu/cu_macro.h>
#include <cu/cu_stl.h>

namespace s2
{

class RenderCtxStack
{
public:
	int  Push(const RenderContext& ctx);
	void Pop();

	const RenderContext* Top() const;

	int Size() const { return m_stack.size(); }

	// todo
	bool Bind(int idx);
	
private:
	static void BindCtx(const RenderContext& ctx);

private:
	CU_VEC<RenderContext> m_stack;

	CU_SINGLETON_DECLARATION(RenderCtxStack)

}; // RenderCtxStack

}

#endif // _SPRITE2_RENDER_CONTEX_STACK_H_