#include "RenderCtxStack.h"
#include "S2_Actor.h"
#include "ActorFactory.h"
#include "S2_Sprite.h"
#include "S2_Symbol.h"
#include "SymType.h"
#include "TextboxActor.h"
#include "ActorLUT.h"
#include "PointQuery2Visitor.h"

#include <iostream>

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
int s2_spr_get_id(void* spr) {
	return static_cast<Sprite*>(spr)->GetID();
}

extern "C"
int s2_spr_get_ref_count(void* spr) {
	return static_cast<Sprite*>(spr)->GetRefCount();
}

extern "C"
const char* s2_spr_get_name(void* spr) {
	return static_cast<Sprite*>(spr)->GetName().c_str();	
}

extern "C"
void* s2_point_query_actor(const void* parent_actor, float x, float y, float mat[6]) {
	const s2::Actor* parent = static_cast<const s2::Actor*>(parent_actor);
	s2::PointQuery2Visitor visitor(parent->GetTreePath(), sm::vec2(x, y));
	sm::mat4 _mat;
	parent->GetSpr()->Traverse(visitor, &_mat);
	mat[0] = _mat.x[0];
	mat[1] = _mat.x[1];
	mat[2] = _mat.x[4];
	mat[3] = _mat.x[5];
	mat[4] = _mat.x[12];
	mat[5] = _mat.x[13];	
	return visitor.GetSelectedActor();
}

extern "C"
void* s2_get_actor(const void* parent_actor, void* child_spr) {
	const Actor* parent = static_cast<const Actor*>(parent_actor);
	Sprite* child = static_cast<Sprite*>(child_spr);
	if (parent) {
		return ActorFactory::Instance()->Create(parent->GetTreePath(), child);
	} else {
		s2::SprTreePath path;
		return ActorFactory::Instance()->Create(path, child);
	}
}

extern "C"
int s2_get_actor_count() {
	return ActorLUT::Instance()->Count();
}

extern "C"
void* s2_actor_get_spr(void* actor) {
	return const_cast<Sprite*>(static_cast<Actor*>(actor)->GetSpr());
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

extern "C"
void s2_actor_print_path(void* actor) {
	Actor* s2_actor = static_cast<Actor*>(actor);
	std::cout << s2_actor->GetTreePath() << '\n';
}

}