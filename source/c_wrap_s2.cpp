#include "c_wrap_s2.h"

#include "RenderCtxStack.h"
#include "S2_Actor.h"
#include "ActorFactory.h"
#include "S2_Sprite.h"
#include "S2_Symbol.h"
#include "SymType.h"
#include "TextboxActor.h"
#include "PointQueryVisitor.h"
#include "SpriteTestVisitor.h"
#include "RenderParams.h"
#include "UpdateParams.h"
#include "DrawNode.h"
#include "RenderFilter.h"
#include "BoundingBox.h"
#include "S2_RVG.h"
#include "SprTimer.h"
#include "SprVisitorParams.h"
#include "S2_RenderTargetMgr.h"
#include "S2_RenderTarget.h"
#include "RenderScissor.h"
#include "Blackboard.h"
#include "s2_trans_color.h"

#include "ComplexSymbol.h"
#include "ComplexSprite.h"
#include "ComplexActor.h"
#include "AnimSymbol.h"
#include "AnimSprite.h"
#include "Anim2Sprite.h"
#include "TextboxSprite.h"
#include "Scale9Sprite.h"
#include "Scale9Actor.h"
#include "AnchorSprite.h"
#include "AnchorActor.h"
#include "Particle3dSprite.h"
#include "Particle3dEmitter.h"
#include "OrthoCamera.h"

#include <shaderlab/ShaderMgr.h>
#include <shaderlab/Sprite2Shader.h>
#include <shaderlab/FilterShader.h>

#include <iostream>
#include <stack>

namespace s2
{

extern "C"
void s2_init()
{
	SprTimer::Instance()->Init();
}

extern "C"
void s2_on_size(int w, int h) 
{
	RenderCtxStack* stack = RenderCtxStack::Instance();
	if (stack->Size() <= 1) {
		stack->Pop();
		stack->Push(RenderContext(w, h, w, h));
	}
}

/************************************************************************/
/* symbol                                                               */
/************************************************************************/

extern "C"
void s2_symbol_draw(const void* actor, float x, float y, float angle, float sx, float sy,
				    float xmin, float ymin, float xmax, float ymax) {
	RenderParams rp;
	rp.mt.SetTransformation(x, y, angle, sx, sy, 0, 0, 0, 0);

	rp.view_region.xmin = xmin;
	rp.view_region.ymin = ymin;
	rp.view_region.xmax = xmax;
	rp.view_region.ymax = ymax;

	const Actor* s2_actor = static_cast<const Actor*>(actor);
	rp.actor = s2_actor;
	const Sprite* s2_spr = static_cast<const Sprite*>(s2_actor->GetSpr());
	DrawNode::Draw(s2_spr->GetSymbol(), rp);
}

/************************************************************************/
/* sprite                                                               */
/************************************************************************/

extern "C"
void s2_spr_draw(const void* actor, float x, float y, float angle, float sx, float sy,
				 float xmin, float ymin, float xmax, float ymax)
{
	const Actor* s2_actor = static_cast<const Actor*>(actor);

	RenderParams rp;
	rp.mt.SetTransformation(x, y, angle, sx, sy, 0, 0, 0, 0);

	rp.view_region.xmin = xmin;
	rp.view_region.ymin = ymin;
	rp.view_region.xmax = xmax;
	rp.view_region.ymax = ymax;

	rp.actor = s2_actor;

	DrawNode::Draw(s2_actor->GetSpr(), rp);
}

extern "C"
void s2_spr_set_pos(void* spr, float x, float y) {
	static_cast<Sprite*>(spr)->SetPosition(sm::vec2(x, y));
}

extern "C"
void s2_spr_set_angle(void* spr, float angle) {
	static_cast<Sprite*>(spr)->SetAngle(angle);
}

extern "C"
void s2_spr_set_scale(void* spr, float sx, float sy) {
	static_cast<Sprite*>(spr)->SetScale(sm::vec2(sx, sy));
}

extern "C"
void s2_spr_get_pos(const void* spr, float* x, float* y) {
	const Sprite* s2_spr = static_cast<const Sprite*>(spr);
	*x = s2_spr->GetPosition().x;
	*y = s2_spr->GetPosition().y;
}

extern "C"
void s2_spr_get_angle(const void* spr, float* angle) {
	const Sprite* s2_spr = static_cast<const Sprite*>(spr);
	*angle = s2_spr->GetAngle();
}

extern "C"
void s2_spr_get_scale(const void* spr, float* sx, float* sy) {
	const Sprite* s2_spr = static_cast<const Sprite*>(spr);
	*sx = s2_spr->GetScale().x;
	*sy = s2_spr->GetScale().y;
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
int s2_spr_get_sym_id(void* spr) {
	return static_cast<Sprite*>(spr)->GetSymbol()->GetID();
}

extern "C"
int s2_spr_get_sym_type(void* spr) {
	return static_cast<Sprite*>(spr)->GetSymbol()->Type();
}

static void set_actor_aabb_dirty(const Actor* actor) {
	const Actor* curr = actor;
	while (curr) {
		curr->SetAABBDirty(true);
		curr->GetSpr()->SetBoundingDirty(true);
		curr = curr->GetParent();
	}
}

extern "C"
void s2_spr_set_action(void* actor, const char* action) {
	Actor* s2_actor = static_cast<Actor*>(actor);
	if (s2_actor->GetSpr()->GetSymbol()->Type() != SYM_COMPLEX) {
		return;
	}

	const Actor* s2_parent = s2_actor->GetParent();
	if(s2_parent && s2_parent->GetSpr()->GetSymbol()->Type() == SYM_ANIMATION) {
		const Sprite* child  = s2_actor->GetSpr();
		const Sprite* parent = s2_parent->GetSpr();
		static_cast<AnimSprite*>(const_cast<Sprite*>(parent))->SetChildAction(s2_parent, child->GetSymbol()->GetID(), action);
		return;
	}

	const ComplexSymbol* sym_complex = VI_DOWNCASTING<const ComplexSymbol*>(s2_actor->GetSpr()->GetSymbol());
	ComplexActor* actor_complex = static_cast<ComplexActor*>(s2_actor);
	int action_idx = -1;
	if (action) {
		action_idx = sym_complex->GetActionIdx(action);
	}
	actor_complex->SetAction(action_idx);

	set_actor_aabb_dirty(s2_actor);
}

extern "C"
int s2_spr_get_frame_count(void* spr) {
	Sprite* s2_spr = static_cast<Sprite*>(spr);
	if (s2_spr->GetSymbol()->Type() == SYM_ANIMATION) {
		AnimSprite* anim = VI_DOWNCASTING<AnimSprite*>(s2_spr);
		const AnimSymbol* sym = VI_DOWNCASTING<const AnimSymbol*>(anim->GetSymbol());
		return sym->GetMaxFrameIdx();
	} else {
		return -1;
	}
}

extern "C"
void s2_spr_draw_aabb(const void* spr, float x, float y, float angle, float sx, float sy, const float mat[6]) {
	S2_MAT outer;
#ifdef S2_MATRIX_FIX
	outer.x[0] = mat[0] * sm::MatrixFix::SCALE;
	outer.x[1] = mat[1] * sm::MatrixFix::SCALE;
	outer.x[2] = mat[2] * sm::MatrixFix::SCALE;
	outer.x[3] = mat[3] * sm::MatrixFix::SCALE;
	outer.x[4] = mat[4] * sm::MatrixFix::TRANSLATE_SCALE;
	outer.x[5] = mat[5] * sm::MatrixFix::TRANSLATE_SCALE;
#else
	outer.x[0] = mat[0];
	outer.x[1] = mat[1];
	outer.x[4] = mat[2];
	outer.x[5] = mat[3];
	outer.x[12]= mat[4];
	outer.x[13]= mat[5];
#endif // S2_MATRIX_FIX

	S2_MAT m;
	m.SetTransformation(x, y, angle, sx, sy, 0, 0, 0, 0);
	m = outer * m;

	const Sprite* s2_spr = static_cast<const Sprite*>(spr);
	sm::rect sz = s2_spr->GetSymbol()->GetBounding(s2_spr);
	std::vector<sm::vec2> vertices(4);
	vertices[0] = sm::vec2(sz.xmin, sz.ymin);
	vertices[1] = sm::vec2(sz.xmin, sz.ymax);
	vertices[2] = sm::vec2(sz.xmax, sz.ymax);
	vertices[3] = sm::vec2(sz.xmax, sz.ymin);
	for (int i = 0; i < 4; ++i) {
		vertices[i] = m * vertices[i];
	}
	RVG::Polyline(vertices, true);
}

extern "C"
bool s2_spr_point_test(const void* spr, float x, float y) {
	const Sprite* s2_spr = static_cast<const Sprite*>(spr);
	return s2_spr->GetBounding()->IsContain(sm::vec2(x, y));
}

extern "C"
void* s2_spr_point_query(const void* spr, float x, float y, float mat[6]) {
	const Sprite* s2_spr = static_cast<const Sprite*>(spr);

	PointQueryVisitor visitor(sm::vec2(x, y));

	SprVisitorParams params;
	s2_spr->Traverse(visitor, params);
	const Actor* ret = visitor.GetSelectedActor();
	if (!ret) {
		return NULL;
	}

	const S2_MAT& selected_mat = visitor.GetSelectedMat();
#ifdef S2_MATRIX_FIX
	mat[0] = selected_mat.x[0] * sm::MatrixFix::SCALE_INV;
	mat[1] = selected_mat.x[1] * sm::MatrixFix::SCALE_INV;
	mat[2] = selected_mat.x[2] * sm::MatrixFix::SCALE_INV;
	mat[3] = selected_mat.x[3] * sm::MatrixFix::SCALE_INV;
	mat[4] = selected_mat.x[4] * sm::MatrixFix::TRANSLATE_SCALE_INV;
	mat[5] = selected_mat.x[5] * sm::MatrixFix::TRANSLATE_SCALE_INV;
#else
	mat[0] = selected_mat.x[0];
	mat[1] = selected_mat.x[1];
	mat[2] = selected_mat.x[4];
	mat[3] = selected_mat.x[5];
	mat[4] = selected_mat.x[12];
	mat[5] = selected_mat.x[13];
#endif // S2_MATRIX_FIX

	return const_cast<Actor*>(ret);
}

extern "C"
bool s2_spr_has_action(const void* spr, const char* name) {
	const Sprite* s2_spr = static_cast<const Sprite*>(spr);
	const ComplexSymbol* sym = static_cast<const ComplexSymbol*>(s2_spr->GetSymbol());
	const std::vector<ComplexSymbol::Action>& actions = sym->GetActions();
	for (int i = 0, n = actions.size(); i < n; ++i) {
		if (name == actions[i].name) {
			return true;
		}
	}
	return false;
}

extern "C"
bool s2_spr_get_scissor(const void* spr, float* xmin, float* ymin, float* xmax, float* ymax)
{
	const Sprite* s2_spr = static_cast<const Sprite*>(spr);
	if (s2_spr->GetSymbol()->Type() != SYM_COMPLEX) {
		*xmin = *ymin = *xmax = *ymax = 0;
		return false;
	}

	const ComplexSprite* complex_spr = static_cast<const ComplexSprite*>(s2_spr);
	const ComplexSymbol* sym = static_cast<const ComplexSymbol*>(complex_spr->GetSymbol());
	const sm::rect& scissor = sym->GetScissor();
	*xmin = scissor.xmin;
	*ymin = scissor.ymin;
	*xmax = scissor.xmax;
	*ymax = scissor.ymax;

	return true;
}

extern "C"
bool s2_spr_set_scissor(void* spr, float xmin, float ymin, float xmax, float ymax)
{
	const Sprite* s2_spr = static_cast<const Sprite*>(spr);
	if (s2_spr->GetSymbol()->Type() != SYM_COMPLEX) {
		return false;
	}

	const ComplexSprite* complex_spr = static_cast<const ComplexSprite*>(s2_spr);
	const ComplexSymbol* sym = static_cast<const ComplexSymbol*>(complex_spr->GetSymbol());
	sm::rect scissor;
	scissor.xmin = xmin;
	scissor.ymin = ymin;
	scissor.xmax = xmax;
	scissor.ymax = ymax;
	const_cast<ComplexSymbol*>(sym)->SetScissor(scissor);

	return true;
}

extern "C"
void s2_spr_textbox_reset_time(void* spr)
{
	Sprite* s2_spr = static_cast<Sprite*>(spr);
	if (s2_spr->GetSymbol()->Type() != SYM_TEXTBOX) {
		return;
	}

	TextboxSprite* tb_spr = static_cast<TextboxSprite*>(s2_spr);
	tb_spr->ResetTime();
}

extern "C"
void s2_spr_anim2_set_static_time(void* spr, int time) 
{
	Sprite* s2_spr = static_cast<Sprite*>(spr);
	if (s2_spr->GetSymbol()->Type() != SYM_ANIM2) {
		return;
	}

	Anim2Sprite* a2_spr = static_cast<Anim2Sprite*>(s2_spr);
	a2_spr->SetStaticTime(time);
}

extern "C"
void s2_spr_p3d_set_local(void* spr, bool local) 
{
	Sprite* s2_spr = static_cast<Sprite*>(spr);
	if (s2_spr->GetSymbol()->Type() != SYM_PARTICLE3D) {
		return;
	}

	Particle3dSprite* p3d_spr = static_cast<Particle3dSprite*>(s2_spr);
	p3d_spr->SetLocal(local);
}

extern "C"
void s2_spr_p3d_set_loop(void* spr, bool loop)
{
	Sprite* s2_spr = static_cast<Sprite*>(spr);
	if (s2_spr->GetSymbol()->Type() != SYM_PARTICLE3D) {
		return;
	}

	Particle3dSprite* p3d_spr = static_cast<Particle3dSprite*>(s2_spr);
	p3d_spr->SetLoop(loop);
}

extern "C"
bool s2_spr_p3d_is_finished(const void* spr) 
{
	const Sprite* s2_spr = static_cast<const Sprite*>(spr);
	if (s2_spr->GetSymbol()->Type() != SYM_PARTICLE3D) {
		return true;
	}

	const Particle3dSprite* p3d_spr = static_cast<const Particle3dSprite*>(s2_spr);
	if (const Particle3dEmitter* et = p3d_spr->GetEmitter()) {
		return et->IsFinished();
	} else {
		return true;
	}
}

extern "C"
void s2_spr_p3d_start(void* spr)
{
	Sprite* s2_spr = static_cast<Sprite*>(spr);
	if (s2_spr->GetSymbol()->Type() != SYM_PARTICLE3D) {
		return;
	}

	Particle3dSprite* p3d_spr = static_cast<Particle3dSprite*>(s2_spr);
	if (const Particle3dEmitter* et = p3d_spr->GetEmitter()) {
		const_cast<Particle3dEmitter*>(et)->Start();
	}
}

extern "C"
void s2_spr_p3d_stop(void* spr)
{
	Sprite* s2_spr = static_cast<Sprite*>(spr);
	if (s2_spr->GetSymbol()->Type() != SYM_PARTICLE3D) {
		return;
	}

	Particle3dSprite* p3d_spr = static_cast<Particle3dSprite*>(s2_spr);
	if (const Particle3dEmitter* et = p3d_spr->GetEmitter()) {
		const_cast<Particle3dEmitter*>(et)->Stop();
	}
}

extern "C"
void s2_spr_p3d_update(void* spr, float dt)
{
	Sprite* s2_spr = static_cast<Sprite*>(spr);
	if (s2_spr->GetSymbol()->Type() != SYM_PARTICLE3D) {
		return;
	}

	Particle3dSprite* p3d_spr = static_cast<Particle3dSprite*>(s2_spr);
	if (const Particle3dEmitter* et = p3d_spr->GetEmitter()) {
		const_cast<Particle3dEmitter*>(et)->Update(et->GetTime() + dt);
	}
}

extern "C"
void s2_spr_p3d_buffer_draw(float x, float y, float scale)
{
	Particle3d::Instance()->BufferDraw(x, y, scale);
}

extern "C"
void s2_spr_set_dtex_enable(void* spr, bool enable)
{
	static_cast<Sprite*>(spr)->SetDTexDisable(!enable);
}

extern "C"
void s2_spr_set_dtex_force_cached(void* spr, bool cache)
{
	Sprite* s2_spr = static_cast<Sprite*>(spr);
	s2_spr->SetDTexForceCached(cache);
	if (cache) {
		s2_spr->SetDTexForceCachedDirty(true);
	}
}

extern "C"
void s2_spr_set_dtex_force_cached_dirty(void* spr, bool dirty)
{
	static_cast<Sprite*>(spr)->SetDTexForceCachedDirty(dirty);
}

/************************************************************************/
/* actor                                                                */
/************************************************************************/

extern "C"
void s2_actor_draw(const void* actor, float x, float y, float angle, float sx, float sy,
				   float xmin, float ymin, float xmax, float ymax) {
	const Actor* s2_actor = static_cast<const Actor*>(actor);

	RenderParams rp;
	rp.view_region.xmin = xmin;
	rp.view_region.ymin = ymin;
	rp.view_region.xmax = xmax;
	rp.view_region.ymax = ymax;

	const Sprite* s2_spr = static_cast<const Sprite*>(s2_actor->GetSpr());

	std::stack<const Actor*> path;
	const Actor* curr = s2_actor;
	while (curr) {
		path.push(curr);
		curr = curr->GetParent();
	}

	RenderParams rp_child(rp);
	while (path.size() > 1) {
		const Actor* curr = path.top();
		path.pop();
		rp.actor = curr;
		DrawNode::Prepare(rp, curr->GetSpr(), rp_child);
		rp = rp_child;
	}

	S2_MAT mt;
	mt.SetTransformation(x, y, angle, sx, sy, 0, 0, 0, 0);

	rp.actor = s2_actor;
	rp.mt = mt * rp.mt;
	DrawNode::Draw(s2_spr, rp);
}

static S2_MAT 
get_actor_world_mat(const Actor* actor) {
	S2_MAT mt;
	const Actor* curr = actor;
	while (curr) {
		mt = mt * curr->GetLocalMat() * curr->GetSpr()->GetLocalMat();
		curr = curr->GetParent();
	}
	return mt;
}

extern "C"
void s2_actor_update(void* actor) {
	const Actor* s2_actor = static_cast<const Actor*>(actor);
	const Sprite* s2_sprite = s2_actor->GetSpr();
	UpdateParams up(s2_actor);
	up.SetPrevMat(get_actor_world_mat(s2_actor->GetParent()));
	const_cast<Sprite*>(s2_sprite)->Update(up);
}

extern "C"
void s2_actor_start(void* actor) {
	const Actor* s2_actor = static_cast<const Actor*>(actor);
	const Sprite* s2_sprite = s2_actor->GetSpr();
	UpdateParams up(s2_actor);
	up.SetPrevMat(get_actor_world_mat(s2_actor->GetParent()));
	const_cast<Sprite*>(s2_sprite)->OnMessage(up, MSG_START);
}

extern "C"
void s2_actor_stop(void* actor) {
	const Actor* s2_actor = static_cast<const Actor*>(actor);
	const Sprite* s2_sprite = s2_actor->GetSpr();
	UpdateParams up(s2_actor);
	up.SetPrevMat(get_actor_world_mat(s2_actor->GetParent()));
	const_cast<Sprite*>(s2_sprite)->OnMessage(up, MSG_STOP);
}

extern "C"
void s2_actor_set_frame(void* actor, int frame) {
	const Actor* s2_actor = static_cast<const Actor*>(actor);
	const Sprite* s2_sprite = s2_actor->GetSpr();
	UpdateParams up(s2_actor);
	up.SetPrevMat(get_actor_world_mat(s2_actor->GetParent()));
	const_cast<Sprite*>(s2_sprite)->SetFrame(up, frame, true);
}

extern "C"
int s2_actor_get_frame(void* actor) {
	const Actor* s2_actor = static_cast<const Actor*>(actor);
	const Sprite* s2_spr = s2_actor->GetSpr();
	if (s2_spr->GetSymbol()->Type() == SYM_ANIMATION) {
		const AnimSprite* anim = VI_DOWNCASTING<const AnimSprite*>(s2_spr);
		return anim->GetFrame(s2_actor);
	} else {
		return -1;
	}
}

extern "C"
int s2_actor_get_component_count(void* actor) {
	int ret = -1;
	const Actor* s2_actor = static_cast<const Actor*>(actor);
	const Sprite* s2_spr = s2_actor->GetSpr();
	switch (s2_spr->GetSymbol()->Type())
	{
	case SYM_COMPLEX:
		{
			const ComplexSprite* complex = VI_DOWNCASTING<const ComplexSprite*>(s2_spr);
			const ComplexSymbol* sym = VI_DOWNCASTING<const ComplexSymbol*>(complex->GetSymbol());
			ret = sym->GetActionChildren(complex->GetAction()).size();
		}
		break;
	case SYM_ANIMATION:
		{
			const AnimSprite* anim = VI_DOWNCASTING<const AnimSprite*>(s2_spr);
			ret = anim->GetAnimCurr(s2_actor).GetSlotSize();
		}
		break;
	}
	return ret;
}

extern "C"
void* s2_actor_fetch_child(const void* actor, const char* name) {
	const Actor* s2_actor = static_cast<const Actor*>(actor);
	const Sprite* child = s2_actor->GetSpr()->FetchChild(name, s2_actor);
	if (child) {
		return const_cast<Sprite*>(child);
	} else {
		return NULL;
	}
}

extern "C"
void* s2_actor_fetch_child_by_index(const void* actor, int idx) {
	const Actor* s2_actor = static_cast<const Actor*>(actor);
	const Sprite* child = s2_actor->GetSpr()->FetchChild(idx, s2_actor);
	if (child) {
		return const_cast<Sprite*>(child);
	} else {
		return NULL;
	}
}

// ret: 0 ok, -1 no child with name, -2 child isn't anchor
extern "C"
int s2_actor_mount(const void* parent, const char* name, const void* child) {
	const Actor* p_actor = static_cast<const Actor*>(parent);
	const Sprite* p_spr = p_actor->GetSpr();
	const Actor* c_actor = static_cast<const Actor*>(child);
	Sprite* c_spr = p_spr->FetchChild(name, p_actor);
	if (!c_spr) {
		return -1;		
	}
	if (c_spr->GetSymbol()->Type() != SYM_ANCHOR) {
		return -2;
	}

	AnchorSprite* anchor_spr = VI_DOWNCASTING<AnchorSprite*>(c_spr);
	anchor_spr->AddAnchor(c_actor, p_actor);
	set_actor_aabb_dirty(p_actor);
	return 0;
}

extern "C"
bool s2_actor_get_force_up_frame(void* actor) {
	const Actor* s2_actor = static_cast<const Actor*>(actor);
	const Sprite* s2_spr = s2_actor->GetSpr();
	if (s2_spr->GetSymbol()->Type() == SYM_ANCHOR) {
		const AnchorSprite* anchor_spr = VI_DOWNCASTING<const AnchorSprite*>(s2_spr);
		const Actor* real = anchor_spr->QueryAnchor(s2_actor);
		if (real) {
			s2_spr = real->GetSpr();
		} else {
			return false;
		}
	}
	return s2_spr->IsForceUpFrame();
}

extern "C"
void s2_actor_set_force_up_frame(void* actor, bool force) {
	const Actor* s2_actor = static_cast<const Actor*>(actor);
	const Sprite* s2_spr = s2_actor->GetSpr();
	if (s2_spr->GetSymbol()->Type() == SYM_ANCHOR) {
		const AnchorSprite* anchor_spr = VI_DOWNCASTING<const AnchorSprite*>(s2_spr);
		const Actor* real = anchor_spr->QueryAnchor(s2_actor);
		if (real) {
			s2_spr = real->GetSpr();
		}
	}
	s2_spr->SetForceUpFrame(force);
}

extern "C"
void* s2_sprite_test_actor(const void* parent_actor, float x, float y) {
	const Actor* parent = static_cast<const Actor*>(parent_actor);

	SprVisitorParams params;
	params.actor = parent;

	SpriteTestVisitor visitor(sm::vec2(x, y));
	parent->GetSpr()->Traverse(visitor, params);

	return const_cast<Actor*>(visitor.GetSelectedActor());
}

extern "C"
void* s2_point_query_actor(const void* parent_actor, float x, float y, float mat[6]) {
	const Actor* parent = static_cast<const Actor*>(parent_actor);

	SprVisitorParams params;
	params.actor = parent;

	PointQueryVisitor visitor(sm::vec2(x, y));
	parent->GetSpr()->Traverse(visitor, params);

	const S2_MAT& selected_mat = visitor.GetSelectedMat();
#ifdef S2_MATRIX_FIX
	mat[0] = selected_mat.x[0] * sm::MatrixFix::SCALE_INV;
	mat[1] = selected_mat.x[1] * sm::MatrixFix::SCALE_INV;
	mat[2] = selected_mat.x[2] * sm::MatrixFix::SCALE_INV;
	mat[3] = selected_mat.x[3] * sm::MatrixFix::SCALE_INV;
	mat[4] = selected_mat.x[4] * sm::MatrixFix::TRANSLATE_SCALE_INV;
	mat[5] = selected_mat.x[5] * sm::MatrixFix::TRANSLATE_SCALE_INV;
#else
	mat[0] = selected_mat.x[0];
	mat[1] = selected_mat.x[1];
	mat[2] = selected_mat.x[4];
	mat[3] = selected_mat.x[5];
	mat[4] = selected_mat.x[12];
	mat[5] = selected_mat.x[13];
#endif // S2_MATRIX_FIX

	return const_cast<Actor*>(visitor.GetSelectedActor());
}

extern "C"
void* s2_get_actor(const void* parent_actor, void* child_spr) {
	const Actor* parent = static_cast<const Actor*>(parent_actor);
	Sprite* child = static_cast<Sprite*>(child_spr);
	return ActorFactory::Instance()->Create(parent, child);
}

extern "C"
int s2_get_actor_count() {
	return -1;
}

extern "C"
void* s2_actor_get_spr(void* actor) {
	return const_cast<Sprite*>(static_cast<Actor*>(actor)->GetSpr());
}

extern "C"
void s2_actor_get_aabb(const void* actor, float aabb[4]) {
	const Actor* s2_actor = static_cast<const Actor*>(actor);
	sm::rect sz = s2_actor->GetSpr()->GetBounding(s2_actor)->GetSize();
	aabb[0] = sz.xmin;
	aabb[1] = sz.ymin;
	aabb[2] = sz.xmax;
	aabb[3] = sz.ymax;
}

extern "C"
void s2_actor_set_pos(void* actor, float x, float y) {
	Actor* s2_actor = static_cast<Actor*>(actor);
	s2_actor->SetPosition(sm::vec2(x, y));
	set_actor_aabb_dirty(s2_actor);
}

extern "C"
void s2_actor_get_pos(void* actor, float* x, float* y) {
	Actor* s2_actor = static_cast<Actor*>(actor);
	sm::vec2 pos = s2_actor->GetPosition();
	*x = pos.x;
	*y = pos.y;
}

extern "C"
void s2_actor_set_angle(void* actor, float angle) {
	Actor* s2_actor = static_cast<Actor*>(actor);
	s2_actor->SetAngle(angle);
	set_actor_aabb_dirty(s2_actor);
}

extern "C"
float s2_actor_get_angle(void* actor) {
	Actor* s2_actor = static_cast<Actor*>(actor);
	return s2_actor->GetAngle();
}

extern "C"
void s2_actor_set_scale(void* actor, float sx, float sy) {
	Actor* s2_actor = static_cast<Actor*>(actor);
	s2_actor->SetScale(sm::vec2(sx, sy));
	set_actor_aabb_dirty(s2_actor);
}

extern "C"
void s2_actor_get_scale(void* actor, float* sx, float* sy) {
	Actor* s2_actor = static_cast<Actor*>(actor);
	sm::vec2 scale = s2_actor->GetScale();
	*sx = scale.x;
	*sy = scale.y;
}

static S2_MAT _get_actor_world_mat(const void* actor) {
	const Actor* s2_actor = static_cast<const Actor*>(actor);
	S2_MAT mat;
	while (s2_actor) {
		mat = mat * s2_actor->GetSpr()->GetLocalMat();
		mat = s2_actor->GetLocalMat() * mat;
		s2_actor = s2_actor->GetParent();
	}
	return mat;
}

extern "C"
void s2_actor_get_world_pos(const void* actor, float* x, float* y) {
	const Actor* s2_actor = static_cast<const Actor*>(actor);
	S2_MAT mat = _get_actor_world_mat(s2_actor);
 	sm::vec2 pos = mat * sm::vec2(0, 0);
 	*x = pos.x;
 	*y = pos.y;
}

extern "C"
float s2_actor_get_world_angle(const void* actor) {
	const Actor* s2_actor = static_cast<const Actor*>(actor);
	S2_MAT mat = _get_actor_world_mat(s2_actor);
 	sm::vec2 pos = mat * sm::vec2(0, 0);
 	sm::vec2 dir = mat * sm::vec2(1, 0);
 	return sm::get_line_angle(pos, dir);
}

extern "C"
void s2_actor_get_world_scale(const void* actor, float* sx, float* sy) {
	const Actor* s2_actor = static_cast<const Actor*>(actor);
	S2_MAT mat = _get_actor_world_mat(s2_actor);
 	sm::vec2 scale = (mat * sm::vec2(1, 1)) - (mat * sm::vec2(0, 0));
 	*sx = scale.x;
 	*sy = scale.y;
}

extern "C"
void* s2_actor_get_parent(void* actor) {
	Actor* s2_actor = static_cast<Actor*>(actor);
	return const_cast<Actor*>(s2_actor->GetParent());
}

extern "C"
bool s2_actor_get_visible(void* actor) {
	Actor* s2_actor = static_cast<Actor*>(actor);
	return s2_actor->IsVisible();
}

extern "C"
void s2_actor_set_visible(void* actor, bool visible) {
	Actor* s2_actor = static_cast<Actor*>(actor);
	s2_actor->SetVisible(visible);
}

extern "C"
bool s2_actor_get_editable(void* actor) {
	Actor* s2_actor = static_cast<Actor*>(actor);
	return s2_actor->IsEditable();
}

extern "C"
void s2_actor_set_editable(void* actor, bool editable) {
	Actor* s2_actor = static_cast<Actor*>(actor);
	s2_actor->SetEditable(editable);
}

extern "C"
uint32_t s2_actor_get_col_mul(void* actor) {
	Actor* s2_actor = static_cast<Actor*>(actor);
	return s2_actor->GetColor().GetMulABGR();
}

extern "C"
void s2_actor_set_col_mul(void* actor, uint32_t abgr) {
	Actor* s2_actor = static_cast<Actor*>(actor);
	if (s2_actor->GetColor().GetMulABGR() == abgr) {
		return;
	}

	RenderColor rc = s2_actor->GetColor();
	rc.SetMulABGR(abgr);
	s2_actor->SetColor(rc);
}

extern "C"
uint32_t s2_actor_get_col_add(void* actor) {
	Actor* s2_actor = static_cast<Actor*>(actor);
	return s2_actor->GetColor().GetAddABGR();
}

extern "C"
void s2_actor_set_col_add(void* actor, uint32_t abgr) {
	Actor* s2_actor = static_cast<Actor*>(actor);
	if (s2_actor->GetColor().GetAddABGR() == abgr) {
		return;
	}

	RenderColor rc = s2_actor->GetColor();
	rc.SetAddABGR(abgr);
	s2_actor->SetColor(rc);
}

extern "C"
void s2_actor_get_col_map(void* actor, uint32_t* rmap, uint32_t* gmap, uint32_t* bmap) {
	Actor* s2_actor = static_cast<Actor*>(actor);
	*rmap = s2_actor->GetColor().GetRMapABGR();
	*gmap = s2_actor->GetColor().GetGMapABGR();
	*bmap = s2_actor->GetColor().GetBMapABGR();
}

extern "C"
void s2_actor_set_col_map(void* actor, uint32_t rmap, uint32_t gmap, uint32_t bmap) {
	Actor* s2_actor = static_cast<Actor*>(actor);
	if (s2_actor->GetColor().GetRMapABGR() == rmap &&
		s2_actor->GetColor().GetGMapABGR() == gmap &&
		s2_actor->GetColor().GetBMapABGR() == bmap) {
		return;
	}

	RenderColor rc = s2_actor->GetColor();
	rc.SetRMapABGR(rmap);
	rc.SetGMapABGR(gmap);
	rc.SetBMapABGR(bmap);
	s2_actor->SetColor(rc);
}

extern "C"
void s2_actor_set_filter(void* actor, int mode) {
	Actor* s2_actor = static_cast<Actor*>(actor);
	const RenderFilter* filter = s2_actor->GetShader().GetFilter();
	FilterMode ori = FM_NULL;
	if (filter) {
		ori = filter->GetMode();
	}
	if (ori == mode) {
		return;
	}

	RenderShader shader = s2_actor->GetShader();
	shader.SetFilter(FilterMode(mode));
	s2_actor->SetShader(shader);
}

extern "C"
const char* s2_actor_get_text(void* actor) {
	Actor* s2_actor = static_cast<Actor*>(actor);
	if (s2_actor->GetSpr()->GetSymbol()->Type() != SYM_TEXTBOX) {
		return NULL;
	}

	TextboxActor* textbox = static_cast<TextboxActor*>(s2_actor);
	return textbox->GetText().c_str();
}

extern "C"
void s2_actor_set_text(void* actor, const char* text) {	
	Actor* s2_actor = static_cast<Actor*>(actor);
	if (s2_actor->GetSpr()->GetSymbol()->Type() != SYM_TEXTBOX) {
		return;
	}

	TextboxActor* textbox = static_cast<TextboxActor*>(s2_actor);
	textbox->SetText(text);
	set_actor_aabb_dirty(s2_actor);
}

extern "C"
bool s2_actor_get_text_size(const void* actor, float* w, float* h) {
	const Actor* s2_actor = static_cast<const Actor*>(actor);
	if (s2_actor->GetSpr()->GetSymbol()->Type() != SYM_TEXTBOX) {
		return false;
	}

	const TextboxActor* textbox = static_cast<const TextboxActor*>(s2_actor);
	const sm::vec2& size = textbox->GetSize();
	*w = size.x;
	*h = size.y;

	return true;
}

extern "C"
void* s2_actor_get_anchor_real(void* actor) {
	const Actor* s2_actor = static_cast<const Actor*>(actor);
	if (s2_actor->GetSpr()->GetSymbol()->Type() != SYM_ANCHOR) {
		return actor;
	}

	const AnchorActor* anchor_actor = VI_DOWNCASTING<const AnchorActor*>(s2_actor);
	void* ret = const_cast<Actor*>(anchor_actor->GetAnchor());
	if (ret) {
		return ret;
	} else {
		return actor;
	}
}

extern "C"
void s2_actor_scale9_resize(void* actor, int w, int h) {
	Actor* s2_actor = static_cast<Actor*>(actor);
	if (s2_actor->GetSpr()->GetSymbol()->Type() != SYM_SCALE9) {
		return;
	}

	Scale9Actor* s9_actor = static_cast<Scale9Actor*>(s2_actor);
	s9_actor->Resize(w, h);
}

/************************************************************************/
/* rt                                                                   */
/************************************************************************/

extern "C"
void* s2_rt_fetch()
{
	return RenderTargetMgr::Instance()->Fetch();
}

extern "C"
void s2_rt_return(void* rt)
{
	RenderTarget* s2_rt = static_cast<RenderTarget*>(rt);
	RenderTargetMgr::Instance()->Return(s2_rt);
}

static void _draw(const struct s2_region* dst, const struct s2_region* src, int src_tex_id)
{
	float vertices[8], texcoords[8];

	float v_xmin = dst->xmin * 2 - 1,
		  v_ymin = dst->ymin * 2 - 1,
		  v_xmax = dst->xmax * 2 - 1,
		  v_ymax = dst->ymax * 2 - 1;
	vertices[0] = v_xmin; vertices[1] = v_ymin;
	vertices[2] = v_xmax; vertices[3] = v_ymin;
	vertices[4] = v_xmax; vertices[5] = v_ymax;
	vertices[6] = v_xmin; vertices[7] = v_ymax;

	float t_xmin = src->xmin,
		  t_ymin = src->ymin,
		  t_xmax = src->xmax,
		  t_ymax = src->ymax;
	texcoords[0] = t_xmin; texcoords[1] = t_ymin;
	texcoords[2] = t_xmax; texcoords[3] = t_ymin;
	texcoords[4] = t_xmax; texcoords[5] = t_ymax;
	texcoords[6] = t_xmin; texcoords[7] = t_ymax;	

	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
	switch (mgr->GetShaderType())
	{
	case sl::SPRITE2:
		{
			sl::Sprite2Shader* shader = static_cast<sl::Sprite2Shader*>(mgr->GetShader());
			shader->SetColor(0xffffffff, 0);
			shader->SetColorMap(0x000000ff, 0x0000ff00, 0x00ff0000);
			shader->Draw(vertices, texcoords, src_tex_id);
		}
		break;
	case sl::FILTER:
		{
			sl::FilterShader* shader = static_cast<sl::FilterShader*>(mgr->GetShader());
			shader->SetColor(0xffffffff, 0);
			shader->Draw(vertices, texcoords, src_tex_id);
		}
		break;
	}
}

extern "C"
void s2_rt_draw_from(void* rt, const struct s2_region* dst, const struct s2_region* src, int src_tex_id)
{
	RenderTargetMgr* RT = RenderTargetMgr::Instance();

	RenderScissor::Instance()->Close();
	RenderCtxStack::Instance()->Push(RenderContext(2, 2, RT->WIDTH, RT->HEIGHT));

	RenderTarget* s2_rt = static_cast<RenderTarget*>(rt);
	s2_rt->Bind();

	_draw(dst, src, src_tex_id);

	s2_rt->Unbind();

	RenderCtxStack::Instance()->Pop();
	RenderScissor::Instance()->Open();
}

extern "C"
void s2_rt_draw_to(void* rt, const struct s2_region* dst, const struct s2_region* src)
{
	RenderScissor::Instance()->Close();
	RenderCtxStack::Instance()->Push(RenderContext(2, 2, 0, 0));

	RenderTarget* s2_rt = static_cast<RenderTarget*>(rt);
	int src_tex_id = s2_rt->GetTexID();
	_draw(dst, src, src_tex_id);

	RenderCtxStack::Instance()->Pop();
	RenderScissor::Instance()->Open();
}

extern "C"
int s2_rt_get_texid(void* rt)
{
	RenderTarget* s2_rt = static_cast<RenderTarget*>(rt);
	return s2_rt->GetTexID();
}

/************************************************************************/
/* camera                                                               */
/************************************************************************/

extern "C"
void* s2_cam_create()
{
	return new OrthoCamera();
}

extern "C"
void s2_cam_release(void* cam)
{
	Camera* c = static_cast<Camera*>(cam);
	delete c;
}

extern "C"
void s2_cam_bind(void* cam)
{
	Camera* c = static_cast<Camera*>(cam);
	c->Bind();
	Blackboard::Instance()->SetCamera(c);
}

extern "C"
void s2_cam_set(void* cam, float x, float y, float scale)
{
	OrthoCamera* o_cam = static_cast<OrthoCamera*>(cam);
	o_cam->Set(sm::vec2(x, y), scale);
}

/************************************************************************/
/* other                                                                */
/************************************************************************/

extern "C"
uint32_t s2_trans_color(uint32_t src, enum S2_PIXEL_TYPE src_type, enum S2_PIXEL_TYPE dst_type)
{
	return trans_color(src, PIXEL_TYPE(src_type), PIXEL_TYPE(dst_type));
}

}
