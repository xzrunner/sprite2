#include "c_wrap_s2.h"

#include "RenderCtxStack.h"

namespace s2
{

extern "C"
void s2_on_size(int w, int h) 
{
	RenderCtxStack* stack = RenderCtxStack::Instance();
	stack->Pop();
	stack->Push(RenderCtx(w, h));
}

}