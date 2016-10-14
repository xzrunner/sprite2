#include "Animation2.h"
#include "DrawNode.h"
#include "S2_Symbol.h"

#include <rigging/rg_skeleton.h>

namespace s2
{

SINGLETON_DEFINITION(Animation2)

Animation2::Animation2()
{
	Init();
}

static void
render_func(void* sym, float x, float y, float angle, float sx, float sy) {
	Symbol* s2_sym = static_cast<Symbol*>(sym);
	DrawNode::Draw(s2_sym, RenderParams(), sm::vec2(x, y), angle, sm::vec2(sx, sy), sm::vec2(0, 0));
}

void Animation2::Init()
{
	rg_skeleton_init(render_func);
}

}