#include "c_wrap_s2.h"

#include "RenderCtxStack.h"

namespace s2
{

extern "C"
void s2_on_size(int w, int h) 
{
	RenderCtxStack* stack = RenderCtxStack::Instance();
	if (stack->Size() <= 1) {
		stack->Pop();
		stack->Push(RenderCtx(w, h, w, h));
	}
}

extern "C"
void s2_render_ctx_push(float w, float h)
{
	RenderCtxStack* stack = RenderCtxStack::Instance();
	RenderCtx ctx(w, h, 0, 0);
	ctx.mv_offset.Set(-w*0.5f, h*0.5f);
	stack->Push(ctx, false);
}

extern "C"
void s2_render_ctx_pop()
{
	RenderCtxStack* stack = RenderCtxStack::Instance();
	stack->Pop(false);
}

}