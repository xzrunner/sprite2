#include "RenderCtxStack.h"
#include "S2_Actor.h"
#include "ActorFactory.h"
#include "S2_Sprite.h"
#include "S2_Symbol.h"
#include "SymType.h"
#include "TextboxActor.h"
#include "ActorLUT.h"

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

extern "C"
void s2_spr_retain(void* spr) {
	Sprite* s2_spr = static_cast<Sprite*>(spr);
	s2_spr->AddReference();
}

extern "C"
void s2_spr_release(void* spr)
{
	Sprite* s2_spr = static_cast<Sprite*>(spr);
	s2_spr->RemoveReference();
}

extern "C"
void* s2_get_actor(const void* parent_actor, void* child_spr) {
	const Actor* parent = static_cast<const Actor*>(parent_actor);
	Sprite* child = static_cast<Sprite*>(child_spr);
	return ActorFactory::Instance()->Create(parent, child);
}

extern "C"
int s2_get_actor_count() {
	return ActorLUT::Instance()->Count();
}

extern "C"
void* s2_actor_get_spr(void* actor) {
	return static_cast<Actor*>(actor)->GetSpr();
}

extern "C"
void s2_actor_set_text(void* actor, const char* text) {	
	Actor* s2_actor = static_cast<Actor*>(actor);
	if (s2_actor->GetSpr()->GetSymbol()->Type() != SYM_TEXTBOX) {
		return;
	}

	TextboxActor* textbox = static_cast<TextboxActor*>(s2_actor);
	textbox->SetText(text);
}

}