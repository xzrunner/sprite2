#include "c_wrap_s2.h"

#include "sprite2/RenderCtxStack.h"
#include "sprite2/Actor.h"
#include "sprite2/ActorFactory.h"
#include "sprite2/Sprite.h"
#include "sprite2/Symbol.h"
#include "sprite2/SymType.h"
#include "sprite2/TextboxActor.h"
#include "sprite2/PointQueryVisitor.h"
#include "sprite2/RenderParams.h"
#include "sprite2/UpdateParams.h"
#include "sprite2/DrawNode.h"
#include "sprite2/BoundingBox.h"
#include "sprite2/RVG.h"
#include "sprite2/SprTimer.h"
#include "sprite2/SprVisitorParams.h"
#include "sprite2/RenderTargetMgr.h"
#include "sprite2/RenderTarget.h"
#include "sprite2/RenderScissor.h"
#include "sprite2/Blackboard.h"
#include "sprite2/trans_color.h"
#include "sprite2/StringHelper.h"
#ifndef S2_DISABLE_STATISTICS
#include "sprite2/StatPingPong.h"
#endif // S2_DISABLE_STATISTICS
#include "sprite2/QueryLoadedVisitor.h"
#include "sprite2/ActorProxy.h"

#include "sprite2/ComplexSymbol.h"
#include "sprite2/ComplexSprite.h"
#include "sprite2/ComplexActor.h"
#include "sprite2/AnimSymbol.h"
#include "sprite2/AnimSprite.h"
#include "sprite2/Anim2Sprite.h"
#include "sprite2/TextboxSprite.h"
#include "sprite2/Scale9Sprite.h"
#include "sprite2/Scale9Actor.h"
#include "sprite2/AnchorSprite.h"
#include "sprite2/AnchorActor.h"
#include "sprite2/Particle3dSprite.h"
#include "sprite2/Particle3dEmitter.h"
#include "sprite2/OrthoCamera.h"
#include "sprite2/ProxySymbol.h"	// todo
#include "sprite2/ProxySprite.h"
#include "sprite2/ProxyHelper.h"

#include <memmgr/Allocator.h>
#include <logger.h>
#include <SM_Math.h>
#include <shaderlab/ShaderMgr.h>
#include <shaderlab/Sprite2Shader.h>
#include <shaderlab/FilterShader.h>
#include <shaderlab/Statistics.h>
#include <c_wrap_cooking.h>
#include <cooking/DisplayList.h>

#include <iostream>
#include <stack>

namespace s2
{

static CU_STR 
_char2string(const char* c_str) 
{
	CU_STR str;
	if (c_str) {
		str.assign(c_str);
	}
	return str;
}

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
		stack->Push(RenderContext(static_cast<float>(w), static_cast<float>(h), w, h));
	}
}

extern "C"
void s2_get_screen_size(int* w, int* h)
{
	const sm::ivec2& sz = Blackboard::Instance()->GetScreenSize();
	*w = sz.x;
	*h = sz.y;
}

/************************************************************************/
/* symbol                                                               */
/************************************************************************/

//extern "C"
//void s2_symbol_draw(const void* actor, float x, float y, float angle, float sx, float sy,
//				    float xmin, float ymin, float xmax, float ymax) {
//	RenderParams rp;
//	rp.mt.SetTransformation(x, y, angle, sx, sy, 0, 0, 0, 0);
//	rp.SetViewRegion(xmin, ymin, xmax, ymax);
//
//	ActorConstPtr s2_actor(*static_cast<const std::shared_ptr<Actor>*>(actor));
//	rp.actor = s2_actor;
//	DrawNode::Draw(s2_actor->GetSprRaw()->GetSymbol(), rp);
//}
//
//extern "C"
//void* s2_symbol_query_child(const void* sym, int child_idx, uint32_t child_id, const char* child_name)
//{
//	const Symbol* s2_sym = static_cast<const Symbol*>(sym);
//	if (s2_sym->Type() != SYM_COMPLEX) {
//		return nullptr;
//	}
//
//	const ComplexSymbol* comp_sym = S2_VI_DOWN_CAST<const ComplexSymbol*>(s2_sym);
//	const CU_VEC<Sprite*>& children = comp_sym->GetAllChildren();
//	if (child_idx >= 0 && child_idx < static_cast<int>(children.size())) {
//		Sprite* child = children[child_idx];
//		assert(child->GetSymbol()->GetID() == child_id);
//		return child;
//	} else {
//		int child_name_id = SprNameMap::Instance()->QueryID(StringHelper::FromChar(child_name));
//		for (int i = 0, n = children.size(); i < n; ++i) {
//			Sprite* child = children[i];
//			if (child->GetSymbol()->GetID() == child_id &&
//				child->GetName() == child_name_id) {
//				return child;
//			}
//		}
//		return nullptr;
//	}
//}
//
//extern "C"
//int s2_symbol_get_type_id(const char* name)
//{
//	int ret = SYM_UNKNOWN;
//	if (strcmp(name, "IMAGE") == 0) {
//		ret = SYM_IMAGE;		
//	} else if (strcmp(name, "SCALE9") == 0) {
//		ret = SYM_SCALE9;
//	} else if (strcmp(name, "ICON") == 0) {
//		ret = SYM_ICON;
//	} else if (strcmp(name, "TEXTURE") == 0) {
//		ret = SYM_TEXTURE;
//	} else if (strcmp(name, "TEXTBOX") == 0) {
//		ret = SYM_TEXTBOX;
//	} else if (strcmp(name, "COMPLEX") == 0) {
//		ret = SYM_COMPLEX;
//	} else if (strcmp(name, "ANIM") == 0) {
//		ret = SYM_ANIMATION;
//	} else if (strcmp(name, "ANIM2") == 0) {
//		ret = SYM_ANIM2;
//	} else if (strcmp(name, "PARTICLE3D") == 0) {
//		ret = SYM_PARTICLE3D;
//	} else if (strcmp(name, "PARTICLE2D") == 0) {
//		ret = SYM_PARTICLE2D;
//	} else if (strcmp(name, "SHAPE") == 0) {
//		ret = SYM_SHAPE;
//	} else if (strcmp(name, "MESH") == 0) {
//		ret = SYM_MESH;
//	} else if (strcmp(name, "MASK") == 0) {
//		ret = SYM_MASK;
//	} else if (strcmp(name, "TRAIL") == 0) {
//		ret = SYM_TRAIL;
//	} else if (strcmp(name, "SKELETON") == 0) {
//		ret = SYM_SKELETON;
//	} else if (strcmp(name, "MODEL") == 0) {
//		ret = SYM_MODEL;
//	}
//	return ret;
//}

/************************************************************************/
/* sprite                                                               */
/************************************************************************/

extern "C"
void s2_spr_draw(const void* actor, float x, float y, float angle, float sx, float sy,
				 float xmin, float ymin, float xmax, float ymax, int flag, int min_edge)
{
	RenderParamsProxy rp_proxy;
	RenderParams* rp = rp_proxy.obj;
	rp->Reset();

	ActorConstPtr s2_actor(*static_cast<const ActorConstPtr*>(actor));

	float* m = rp->mt.x;
	float c, s;
	if (angle == 0) {
		c = 1;
		s = 0;
	} else {
		c = sm::cos_fast(angle);
		s = sm::sin_fast(angle);
	}
	m[0] = c * sx;
	m[1] = s * sx;
	m[2] = - s * sy;
	m[3] = c * sy;
	m[4] = x;
	m[5] = y;

	rp->SetViewRegion(xmin, ymin, xmax, ymax);

	rp->actor = s2_actor.get();

//	rp->SetDisableCulling(true);

	if ((flag & S2_DISABLE_DRAW_PARTICLE3D) == 1) {
		rp->SetDisableParticle3d(true);
	}
	rp->min_edge = min_edge;

	if (s2_actor->GetSprRaw()->IsDTexDisable()) {
		rp->SetDisableDTexC2(true);
	}

#ifdef S2_DISABLE_DEFERRED
	DrawNode::Draw(nullptr, s2_actor->GetSprRaw(), *rp);
#else
	cooking::DisplayList dlist;
	DrawNode::Draw(&dlist, s2_actor->GetSprRaw(), *rp);
	dlist.Replay(-1, -1);
#endif // S2_DISABLE_DEFERRED
}

#ifndef S2_DISABLE_FLATTEN
// todo: build flatten async
static void check_and_build_flatten(const ActorPtr& actor)
{
	if (!actor->HasFlatten()) {
		actor->BuildFlatten();
	}
	if (!actor->HasFlatten()) {
		actor->CreateFlatten();
		actor->BuildFlatten();
	}
	assert(actor->HasFlatten());
}
#endif // S2_DISABLE_FLATTEN

#ifndef S2_DISABLE_FLATTEN
extern "C"
void  s2_spr_draw_ft(const void* actor, float x, float y, float angle, float sx, float sy,
	                 float xmin, float ymin, float xmax, float ymax, int flag, int min_edge)
{
	const ActorPtr& s2_actor(static_cast<const ActorProxy*>(actor)->actor);
	check_and_build_flatten(s2_actor);
	//// todo: build flatten async
	//if (!s2_actor->HasFlatten()) {
	//	return s2_spr_draw(actor, x, y, angle, sx, sy, xmin, ymin, xmax, ymax, flag, min_edge);
	//}

	RenderParamsProxy rp_proxy;
	RenderParams* rp = rp_proxy.obj;
	rp->Reset();

	float* m = rp->mt.x;
	float c, s;
	if (angle == 0) {
		c = 1;
		s = 0;
	} else {
		c = sm::cos_fast(angle);
		s = sm::sin_fast(angle);
	}
	m[0] = c * sx;
	m[1] = s * sx;
	m[2] = -s * sy;
	m[3] = c * sy;
	m[4] = x;
	m[5] = y;

	rp->SetViewRegion(xmin, ymin, xmax, ymax);

	rp->actor = nullptr;

	if (s2_actor->GetSprRaw()->IsDTexDisable()) {
		rp->SetDisableDTexC2(true);
	}

	//	rp->SetDisableCulling(true);

	if ((flag & S2_DISABLE_DRAW_PARTICLE3D) == 1) {
		rp->SetDisableParticle3d(true);
	}
	rp->min_edge = min_edge;

	rp->SetEnableDrawlist(Blackboard::Instance()->IsDlistEnable());

	s2_actor->FlattenDraw(*rp);
}
#endif // S2_DISABLE_FLATTEN

extern "C"
void s2_spr_set_pos(void* actor, float x, float y) {
	ActorPtr& s2_actor(static_cast<ActorProxy*>(actor)->actor);
	auto s2_spr = const_cast<Sprite*>(s2_actor->GetSprRaw());

	s2_spr->SetPosition(sm::vec2(x, y));
}

extern "C"
void s2_spr_set_angle(void* actor, float angle) {
	ActorPtr& s2_actor(static_cast<ActorProxy*>(actor)->actor);
	auto s2_spr = const_cast<Sprite*>(s2_actor->GetSprRaw());

	s2_spr->SetAngle(angle);
}

extern "C"
void s2_spr_set_scale(void* actor, float sx, float sy) {
	ActorPtr& s2_actor(static_cast<ActorProxy*>(actor)->actor);
	auto s2_spr = const_cast<Sprite*>(s2_actor->GetSprRaw());

	s2_spr->SetScale(sm::vec2(sx, sy));
}

extern "C"
void s2_spr_get_pos(const void* actor, float* x, float* y) {
	const ActorPtr& s2_actor(static_cast<const ActorProxy*>(actor)->actor);
	auto s2_spr = const_cast<Sprite*>(s2_actor->GetSprRaw());

	sm::vec2 pos;
	if (ProxyHelper::SprGetPos(*s2_spr, pos)) {
		*x = pos.x;
		*y = pos.y;
	} else {
		LOGW("s2_spr_get_pos fail, sym_id %d", s2_spr->GetSymbol()->GetID());
	}
}

extern "C"
float s2_spr_get_angle(const void* actor) {
	const ActorPtr& s2_actor(static_cast<const ActorProxy*>(actor)->actor);
	auto s2_spr = const_cast<Sprite*>(s2_actor->GetSprRaw());

	float angle = 0;
	if (ProxyHelper::SprGetAngle(*s2_spr, angle)) {
		;
	} else {
		LOGW("s2_spr_get_angle fail, sym_id %d", s2_spr->GetSymbol()->GetID());
	}
	return angle;
}

extern "C"
void s2_spr_get_scale(const void* actor, float* sx, float* sy) {
	const ActorPtr& s2_actor(static_cast<const ActorProxy*>(actor)->actor);
	auto s2_spr = const_cast<Sprite*>(s2_actor->GetSprRaw());

	sm::vec2 scale;
	if (ProxyHelper::SprGetScale(*s2_spr, scale)) {
		*sx = scale.x;
		*sy = scale.y;
	} else {
		LOGW("s2_spr_get_scale fail, sym_id %d", s2_spr->GetSymbol()->GetID());
	}
}

//extern "C"
//void s2_spr_set_col_mul(void* actor, uint32_t abgr) {
//	ActorPtr& s2_actor(static_cast<ActorProxy*>(actor)->actor);
//	auto s2_spr = const_cast<Sprite*>(s2_actor->GetSprRaw());
//
//	RenderColor rc = s2_actor->GetSprRaw()->GetColor();
//	rc.SetMulABGR(abgr);
//	s2_spr->SetColor(rc);
//}
//
//extern "C"
//void s2_spr_set_col_add(void* actor, uint32_t abgr) {
//	ActorPtr& s2_actor(static_cast<ActorProxy*>(actor)->actor);
//	auto s2_spr = const_cast<Sprite*>(s2_actor->GetSprRaw());
//
//	RenderColor rc = s2_spr->GetColor();
//	rc.SetAddABGR(abgr);
//	s2_spr->SetColor(rc);
//}

//extern "C"
//void s2_spr_retain(void* spr) {
//	Sprite* s2_spr = static_cast<Sprite*>(spr);
//	s2_spr->AddReference();
//}
//
//extern "C"
//void s2_spr_release(void* spr)
//{
//	Sprite* s2_spr = static_cast<Sprite*>(spr);
//	s2_spr->RemoveReference();
//}

extern "C"
const char* s2_spr_get_name(void* actor) {
	ActorConstPtr s2_actor(*static_cast<const ActorConstPtr*>(actor));

	CU_STR name;
	SprNameMap::Instance()->IDToStr(s2_actor->GetSprRaw()->GetName(), name);
	char* cstr = (char*)malloc(name.length() + 1);
	strcpy(cstr, name.c_str());
	return cstr;
}

extern "C"
int s2_spr_get_sym_id(const void* actor) {
	const ActorPtr& s2_actor(static_cast<const ActorProxy*>(actor)->actor);
	auto s2_spr = const_cast<Sprite*>(s2_actor->GetSprRaw());

	int id = 0;
	if (ProxyHelper::SprGetSymID(*s2_spr, id)) {
		;
	} else {
		LOGW("s2_spr_get_sym_id fail, sym_id %d", s2_spr->GetSymbol()->GetID());
	}
	return id;
}

extern "C"
int s2_spr_get_sym_type(const void* actor) {
	const ActorPtr& s2_actor(static_cast<const ActorProxy*>(actor)->actor);
	return s2_actor->GetSprRaw()->GetSymbol()->Type();
}

extern "C"
void s2_spr_draw_aabb(const void* actor, float x, float y, float angle, float sx, float sy, const float mat[6]) {
	S2_MAT outer;
#ifdef S2_MATRIX_FIX
	outer.x[0] = mat[0] * sm::MatrixFix::SCALE;
	outer.x[1] = mat[1] * sm::MatrixFix::SCALE;
	outer.x[2] = mat[2] * sm::MatrixFix::SCALE;
	outer.x[3] = mat[3] * sm::MatrixFix::SCALE;
	outer.x[4] = mat[4] * sm::MatrixFix::TRANSLATE_SCALE;
	outer.x[5] = mat[5] * sm::MatrixFix::TRANSLATE_SCALE;
#else
	memcpy(outer.x, mat, sizeof(outer.x));
#endif // S2_MATRIX_FIX

	S2_MAT m;
	m.SetTransformation(x, y, angle, sx, sy, 0, 0, 0, 0);
	m = outer * m;

	const ActorPtr& s2_actor(static_cast<const ActorProxy*>(actor)->actor);
	auto s2_spr = s2_actor->GetSprRaw();

	sm::rect sz = s2_spr->GetSymbol()->GetBounding(s2_spr);
	CU_VEC<sm::vec2> vertices;
	vertices.resize(4);
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
bool s2_spr_point_test(const void* actor, float x, float y) {
	const ActorPtr& s2_actor(static_cast<const ActorProxy*>(actor)->actor);
	auto s2_spr = const_cast<Sprite*>(s2_actor->GetSprRaw());

	return ProxyHelper::SprPointTest(*s2_spr, sm::vec2(x, y));
}

extern "C"
bool s2_spr_get_force_update(const void* actor) {
	const ActorPtr& s2_actor(static_cast<const ActorProxy*>(actor)->actor);
	auto s2_spr = const_cast<Sprite*>(s2_actor->GetSprRaw());

	bool force = false;
	if (ProxyHelper::SprGetForceUpdate(*s2_spr, force)) {
		return force;
	} else {
		LOGW("s2_spr_get_force_update fail, sym_id %d", s2_spr->GetSymbol()->GetID());
		return false;		
	}
}

extern "C"
void s2_spr_set_force_update(void* actor, bool force) {
	ActorPtr& s2_actor(static_cast<ActorProxy*>(actor)->actor);
	auto s2_spr = const_cast<Sprite*>(s2_actor->GetSprRaw());

	ProxyHelper::SprSetForceUpdate(*s2_spr, force);
}

extern "C"
bool s2_spr_is_res_loaded(const void* actor) {
	const ActorPtr& s2_actor(static_cast<const ActorProxy*>(actor)->actor);
	auto s2_spr = const_cast<Sprite*>(s2_actor->GetSprRaw());

	QueryLoadedVisitor visitor;
	s2_spr->Traverse(visitor, SprVisitorParams());
	return visitor.IsLoaded();
}

extern "C"
int s2_spr_get_frame_count(void* actor) {
	ActorPtr& s2_actor(static_cast<ActorProxy*>(actor)->actor);
	auto s2_spr = const_cast<Sprite*>(s2_actor->GetSprRaw());

	int count = -1;
	if (ProxyHelper::SprGetFrameCount(*s2_spr, count)) {
		;
	} else {
		LOGW("s2_spr_get_frame_count fail, sym_id %d", s2_spr->GetSymbol()->GetID());
	}
	return count;
}

extern "C"
void s2_spr_anim_set_loop(void* actor, bool loop)
{
	ActorPtr& s2_actor(static_cast<ActorProxy*>(actor)->actor);
	auto s2_spr = const_cast<Sprite*>(s2_actor->GetSprRaw());

	ProxyHelper::SprAnimSetLoop(*s2_spr, loop);
}

extern "C"
bool s2_spr_has_action(const void* actor, const char* name) {
	const ActorPtr& s2_actor(static_cast<const ActorProxy*>(actor)->actor);

	return ProxyHelper::SprHasAction(*s2_actor->GetSprRaw(), _char2string(name));
}

extern "C"
void s2_spr_textbox_reset_time(void* actor)
{
	ActorPtr& s2_actor(static_cast<ActorProxy*>(actor)->actor);
	auto s2_spr = const_cast<Sprite*>(s2_actor->GetSprRaw());

	ProxyHelper::SprTextboxResetTime(*s2_spr);
}

extern "C"
void s2_spr_textbox_set_font_color(void* actor, uint32_t abgr)
{
	ActorPtr& s2_actor(static_cast<ActorProxy*>(actor)->actor);
	auto s2_spr = const_cast<Sprite*>(s2_actor->GetSprRaw());

	ProxyHelper::SprTextboxSetFontColor(*s2_spr, abgr);
}

extern "C"
void s2_spr_textbox_set_edge_color(void* actor, uint32_t abgr)
{
	ActorPtr& s2_actor(static_cast<ActorProxy*>(actor)->actor);
	auto s2_spr = const_cast<Sprite*>(s2_actor->GetSprRaw());

	ProxyHelper::SprTextboxSetEdgeColor(*s2_spr, abgr);
}

extern "C"
void s2_spr_anim2_set_static_time(void* actor, int time)
{
	ActorPtr& s2_actor(static_cast<ActorProxy*>(actor)->actor);
	auto s2_spr = const_cast<Sprite*>(s2_actor->GetSprRaw());

	ProxyHelper::SprAnim2SetStaticTime(*s2_spr, time);
}

extern "C"
void s2_spr_p3d_set_local(void* actor, bool local)
{
	ActorPtr& s2_actor(static_cast<ActorProxy*>(actor)->actor);
	auto s2_spr = const_cast<Sprite*>(s2_actor->GetSprRaw());

	ProxyHelper::SprP3dSetLocal(*s2_spr, local);
}

extern "C"
void s2_spr_p3d_set_loop(void* actor, bool loop)
{
	ActorPtr& s2_actor(static_cast<ActorProxy*>(actor)->actor);
	auto s2_spr = const_cast<Sprite*>(s2_actor->GetSprRaw());

	ProxyHelper::SprP3dSetLoop(*s2_spr, loop);
}

extern "C"
bool s2_spr_p3d_is_finished(const void* actor)
{
	const ActorPtr& s2_actor(static_cast<const ActorProxy*>(actor)->actor);
	auto s2_spr = const_cast<Sprite*>(s2_actor->GetSprRaw());

	return ProxyHelper::SprP3dIsFinished(*s2_spr);
}

extern "C"
void s2_spr_p3d_update(void* actor, float dt)
{
	ActorPtr& s2_actor(static_cast<ActorProxy*>(actor)->actor);
	auto s2_spr = const_cast<Sprite*>(s2_actor->GetSprRaw());

	ProxyHelper::SprP3dUpdate(*s2_spr, dt);
}

extern "C"
void s2_spr_p3d_set_start_radius(void* actor, float radius)
{
	ActorPtr& s2_actor(static_cast<ActorProxy*>(actor)->actor);
	auto s2_spr = const_cast<Sprite*>(s2_actor->GetSprRaw());

	ProxyHelper::SprP3dSetStartRadius(*s2_spr, radius);
}

extern "C"
void s2_spr_p3d_buffer_draw(float x, float y, float scale)
{
	Particle3d::Instance()->BufferDraw(x, y, scale);
}

extern "C"
void s2_spr_p3d_tick()
{
	Particle3d::Instance()->Tick();
}

extern "C"
void s2_spr_p3d_gc()
{
	Particle3d::Instance()->GC();
}

extern "C"
void s2_spr_proxy_get_children(const void* actor, void* children[], int children_cap, int* count)
{
	const ActorPtr& s2_actor(static_cast<const ActorProxy*>(actor)->actor);

	CU_VEC<ActorPtr> actors;
	ProxyHelper::SprGetProxyChildren(*s2_actor->GetSprRaw(), actors);
	int n = std::min(children_cap, static_cast<int>(actors.size()));
	for (int i = 0; i < n; ++i) {
		ActorProxy* proxy;
		ActorProxyPool::Instance()->Create(actors[i], proxy);
		children[i] = proxy;
	}
	*count = n;
}

extern "C"
void s2_spr_set_dtex_enable(void* actor, bool enable)
{
	ActorPtr& s2_actor(static_cast<ActorProxy*>(actor)->actor);
	auto s2_spr = const_cast<Sprite*>(s2_actor->GetSprRaw());

	ProxyHelper::SetDTexEnable(*s2_spr, enable);
}

extern "C"
void s2_spr_set_dtex_force_cached(void* actor, bool cache)
{
	ActorPtr& s2_actor(static_cast<ActorProxy*>(actor)->actor);
	auto s2_spr = const_cast<Sprite*>(s2_actor->GetSprRaw());

	ProxyHelper::SetDTexForceCached(*s2_spr, cache);
}

extern "C"
void s2_spr_set_dtex_force_cached_dirty(void* actor, bool dirty)
{
	ActorPtr& s2_actor(static_cast<ActorProxy*>(actor)->actor);
	auto s2_spr = const_cast<Sprite*>(s2_actor->GetSprRaw());

	ProxyHelper::SetDTexForceCachedDirty(*s2_spr, dirty);
}

/************************************************************************/
/* actor                                                                */
/************************************************************************/

extern "C"
void* s2_actor_get_real_ptr(void* actor) {
	const ActorPtr& s2_actor(static_cast<const ActorProxy*>(actor)->actor);
	return s2_actor.get();
}

//extern "C"
//void s2_actor_retain(void* actor) {
//	ActorPtr& s2_actor(static_cast<ActorProxy*>(actor)->actor);
//	new s2::ActorPtr(s2_actor);
//}

extern "C"
void s2_actor_release(void* actor) {
	ActorPtr& s2_actor(static_cast<ActorProxy*>(actor)->actor);
	ActorProxyPool::Instance()->Delete(s2_actor);
}

#ifndef S2_DISABLE_FLATTEN
extern "C"
bool s2_actor_has_ft(const void* actor) {
	const ActorPtr& s2_actor(static_cast<const ActorProxy*>(actor)->actor);
	return s2_actor->HasFlatten();
}

extern "C"
void s2_actor_build_ft(void* actor) {
	ActorPtr& s2_actor(static_cast<ActorProxy*>(actor)->actor);
	s2_actor->CreateFlatten();
}

extern "C"
void s2_actor_set_ft_dirty(void* actor) {
	ActorPtr& s2_actor(static_cast<ActorProxy*>(actor)->actor);
	s2_actor->SetFlattenDirty();
}
#endif // S2_DISABLE_FLATTEN

#ifndef S2_DISABLE_DEFERRED
extern "C"
void s2_actor_build_dlist(void* actor) {
	ActorPtr& s2_actor(static_cast<ActorProxy*>(actor)->actor);
	s2_actor->BuildDisplayList();
}
#endif // S2_DISABLE_DEFERRED

extern "C"
void s2_actor_draw(const void* actor, float x, float y, float angle, float sx, float sy,
				   float xmin, float ymin, float xmax, float ymax) {
	const ActorConstPtr& s2_actor(static_cast<const ActorProxy*>(actor)->actor);

	RenderParams rp;
	rp.SetViewRegion(xmin, ymin, xmax, ymax);

	auto s2_spr = s2_actor->GetSprRaw();

	std::stack<ActorConstPtr> path;
	auto curr = s2_actor;
	while (curr) {
		path.push(curr);
		curr = curr->GetParent();
	}

	RenderParamsProxy rp_proxy;
	RenderParams* rp_child = rp_proxy.obj;
	memcpy(rp_child, &rp, sizeof(rp));

	while (path.size() > 1) {
		auto curr = path.top();
		path.pop();
		rp.actor = curr.get();
		DrawNode::Prepare(rp, curr->GetSprRaw(), *rp_child);
		rp = *rp_child;
	}

	S2_MAT mt;
	mt.SetTransformation(x, y, angle, sx, sy, 0, 0, 0, 0);

	rp.actor = s2_actor.get();
	rp.mt = mt * rp.mt;
	DrawNode::Draw(nullptr, s2_spr, rp);
}

#ifndef S2_DISABLE_FLATTEN
extern "C"
void s2_actor_draw_ft(const void* actor, float x, float y, float angle, float sx, float sy,
	                  float xmin, float ymin, float xmax, float ymax) {
	const ActorPtr& s2_actor(static_cast<const ActorProxy*>(actor)->actor);
	check_and_build_flatten(s2_actor);
	//// todo: build flatten async
	//if (!s2_actor->HasFlatten()) {
	//	return s2_actor_draw(actor, x, y, angle, sx, sy, xmin, ymin, xmax, ymax);
	//}

	RenderParams rp;
	rp.SetViewRegion(xmin, ymin, xmax, ymax);

	auto s2_spr = const_cast<Sprite*>(s2_actor->GetSprRaw());

	std::stack<ActorConstPtr> path;
	ActorConstPtr curr = s2_actor;
	while (curr) {
		path.push(curr);
		curr = curr->GetParent();
	}

	RenderParamsProxy rp_proxy;
	RenderParams* rp_child = rp_proxy.obj;
	memcpy(rp_child, &rp, sizeof(rp));

	while (path.size() > 1) {
		auto curr = path.top();
		path.pop();
		rp.actor = curr.get();
		DrawNode::Prepare(rp, curr->GetSprRaw(), *rp_child);
		rp = *rp_child;
	}

	S2_MAT mt;
	mt.SetTransformation(x, y, angle, sx, sy, 0, 0, 0, 0);

	rp.SetEnableDrawlist(Blackboard::Instance()->IsDlistEnable());

	rp.actor = s2_actor.get();
	rp.mt = mt * rp.mt;

	s2_actor->FlattenDraw(rp);
}
#endif // S2_DISABLE_FLATTEN

static S2_MAT 
get_actor_world_mat(const ActorConstPtr& actor) {
	S2_MAT mt;
	auto curr = actor;
	while (curr) {
		if (curr->GetSprRaw()->GetSymbol()->Type() == SYM_MASK) {
			break;
		}
		mt = mt * curr->GetLocalMat() * curr->GetSprRaw()->GetLocalMat();
		curr = curr->GetParent();
	}
	return mt;
}

extern "C"
void s2_actor_update(void* actor, bool force) {
	ActorPtr& s2_actor(static_cast<ActorProxy*>(actor)->actor);
	auto s2_spr = const_cast<Sprite*>(s2_actor->GetSprRaw());

	UpdateParams up(s2_actor.get());
	up.SetPrevMat(get_actor_world_mat(s2_actor->GetParent()));
	up.SetForce(force);

	int old_inherit_update = ProxyHelper::SprGetInheritUpdate(*s2_spr);
	if (old_inherit_update != -1) {
		ProxyHelper::SprSetInheritUpdate(*s2_spr, true);
		s2_spr->Update(up);
		ProxyHelper::SprSetInheritUpdate(*s2_spr, old_inherit_update == 1);
	} else {
		s2_spr->Update(up);
	}
}

#ifndef S2_DISABLE_FLATTEN
extern "C"
void  s2_actor_update_ft(void* actor, bool force) {
	ActorPtr& s2_actor(static_cast<ActorProxy*>(actor)->actor);
	check_and_build_flatten(s2_actor);
	//// todo: build flatten async
	//if (!s2_actor->HasFlatten()) {
	//	return s2_actor_update()
	//}

	const Sprite& spr(*s2_actor->GetSpr());
	int old_inherit_update = ProxyHelper::SprGetInheritUpdate(spr);
	if (old_inherit_update != -1) {
		ProxyHelper::SprSetInheritUpdate(spr, true);
		s2_actor->FlattenUpdate(force);
		ProxyHelper::SprSetInheritUpdate(spr, old_inherit_update == 1);
	} else {
		s2_actor->FlattenUpdate(force);
	}
}
#endif // S2_DISABLE_FLATTEN

static void actor_send_msg(void* actor, bool force, Message msg) 
{
	ActorPtr& s2_actor(static_cast<ActorProxy*>(actor)->actor);

	UpdateParams up(s2_actor.get());
	up.SetPrevMat(get_actor_world_mat(s2_actor->GetParent()));
	up.SetForce(force);

	auto s2_spr = const_cast<Sprite*>(s2_actor->GetSprRaw());
	s2_spr->OnMessage(up, msg);
}

extern "C"
void s2_actor_msg_start(void* actor, bool force) {
	actor_send_msg(actor, force, MSG_START);
}

extern "C"
void s2_actor_msg_play(void* actor, bool force) {
	actor_send_msg(actor, force, MSG_PLAY);
}

extern "C"
void s2_actor_msg_stop(void* actor, bool force) {
	actor_send_msg(actor, force, MSG_STOP);
}

extern "C"
void s2_actor_msg_clear(void* actor, bool force) {
	actor_send_msg(actor, force, MSG_CLEAR);
}

extern "C"
void s2_actor_set_frame(void* actor, int frame) {
	ActorPtr& s2_actor(static_cast<ActorProxy*>(actor)->actor);
	ProxyHelper::ActorSetFrame(s2_actor.get(), frame);
}

#ifndef S2_DISABLE_FLATTEN
extern "C"
void s2_actor_set_frame_ft(void* actor, int frame) {
	ActorPtr& s2_actor(static_cast<ActorProxy*>(actor)->actor);
	check_and_build_flatten(s2_actor);
	//// todo: build flatten async
	//if (!s2_actor->HasFlatten()) {
	//	return s2_actor_set_frame()
	//}

	auto spr = s2_actor->GetSprRaw();
	bool old_inherit_update = spr->IsInheritUpdate();
	spr->SetInheritUpdate(true);
	s2_actor->FlattenSetFrame(frame);
	spr->SetInheritUpdate(old_inherit_update);
}
#endif // S2_DISABLE_FLATTEN

extern "C"
int s2_actor_get_frame(void* actor) {
	ActorPtr& s2_actor(static_cast<ActorProxy*>(actor)->actor);
	int frame = -1;
	if (ProxyHelper::ActorGetFrame(s2_actor.get(), frame)) {
		return frame;
	} else {
		LOGW("s2_actor_get_frame fail, sym_id %d", s2_actor->GetSprRaw()->GetSymbol()->GetID());
		return -1;
	}
}

extern "C"
int s2_actor_get_component_count(void* actor) {
	ActorPtr& s2_actor(static_cast<ActorProxy*>(actor)->actor);
	int count = -1;
	if (ProxyHelper::ActorGetComponentCount(s2_actor.get(), count)) {
		return count;
	} else {
		// LOGW("s2_actor_get_component_count fail, sym_id %d", s2_actor->GetSprRaw()->GetSymbol()->GetID());
		return -1;
	}
}

extern "C"
void* s2_actor_fetch_child(const void* actor, const char* name, bool* is_new) {
	const ActorPtr& s2_actor(static_cast<const ActorProxy*>(actor)->actor);
	auto s2_spr = const_cast<Sprite*>(s2_actor->GetSprRaw());

	int name_id = SprNameMap::Instance()->StrToID(StringHelper::FromChar(name));
	auto child_spr = s2_spr->FetchChildByName(name_id, s2_actor);
	if (!child_spr) {
		*is_new = false;
		return nullptr;
	} else {
		auto child_actor = ActorFactory::Create(s2_actor, child_spr);
		ActorProxy* proxy;
		*is_new = ActorProxyPool::Instance()->Create(child_actor, proxy);
		return proxy;
	}
}

extern "C"
void* s2_actor_fetch_child_by_index(const void* actor, int idx, bool* is_new) {
	const ActorPtr& s2_actor(static_cast<const ActorProxy*>(actor)->actor);
	auto s2_spr = const_cast<Sprite*>(s2_actor->GetSprRaw());

	auto child_spr = s2_spr->FetchChildByIdx(idx, s2_actor);
	if (!child_spr) {
		*is_new = false;
		return nullptr;
	} else {
		auto child_actor = ActorFactory::Create(s2_actor, child_spr);
		ActorProxy* proxy;
		*is_new = ActorProxyPool::Instance()->Create(child_actor, proxy);
		return proxy;
	}
}

static int
_actor_mount(const ActorConstPtr& parent, const SprPtr& old_child, const ActorPtr& new_child) {
	if (!old_child) {
		return -1;		
	}
	int sym_type = old_child->GetSymbol()->Type();
	if (sym_type == SYM_PROXY) 
	{
		auto& items = S2_VI_PTR_DOWN_CAST<ProxySymbol>(old_child->GetSymbol())->GetItems();
		int ret = 0;
		for (int i = 0, n = items.size(); i < n; ++i) {
			int _ret = _actor_mount(items[i].first, items[i].second, new_child);
			if (_ret != 0) {
				ret = _ret;
			}
		}
		return ret;
	} 
	else if (sym_type == SYM_ANCHOR) 
	{
		auto new_actor = ActorFactory::Create(parent, old_child);
		S2_VI_PTR_DOWN_CAST<AnchorActor>(new_actor)->SetAnchor(new_child);
		return 0;
	} 
	else 
	{
		return -2;
	}
}

// ret: 0 ok, -1 no child with name, -2 child isn't anchor
extern "C"
int s2_actor_mount(const void* parent, const char* name, const void* child) {
	const ActorPtr& s2_parent(static_cast<const ActorProxy*>(parent)->actor);
	const ActorPtr& s2_child(child ? static_cast<const ActorProxy*>(child)->actor : nullptr);

	int name_id = SprNameMap::Instance()->StrToID(StringHelper::FromChar(name));
	auto old_spr = s2_parent->GetSprRaw()->FetchChildByName(name_id, s2_parent);
	if (!old_spr) {
		return -1;
	}

	int sym_type = old_spr->GetSymbol()->Type();
	if (sym_type == SYM_PROXY) 
	{
		auto& items = S2_VI_PTR_DOWN_CAST<ProxySymbol>(old_spr->GetSymbol())->GetItems();
		int ret = 0;
		for (auto& item : items) 
		{
			int _ret = _actor_mount(item.first, item.second, s2_child);
			if (_ret != 0) {
				ret = _ret;
			}
		}
		return ret;
	} 
	else if (sym_type == SYM_ANCHOR) 
	{
		return _actor_mount(s2_parent, old_spr, s2_child);
	} 
	else 
	{
		return -2;
	}
}

extern "C"
void* s2_point_query_actor(const void* parent_actor, float x, float y, float mat[6]) {
	ActorPtr s2_parent(*static_cast<const ActorPtr*>(parent_actor));

	SprVisitorParams2 params;
	params.actor = s2_parent;

	PointQueryVisitor visitor(sm::vec2(x, y));
	s2_parent->GetSprRaw()->Traverse2(visitor, params);

	const S2_MAT& selected_mat = visitor.GetSelectedMat();
#ifdef S2_MATRIX_FIX
	mat[0] = selected_mat.x[0] * sm::MatrixFix::SCALE_INV;
	mat[1] = selected_mat.x[1] * sm::MatrixFix::SCALE_INV;
	mat[2] = selected_mat.x[2] * sm::MatrixFix::SCALE_INV;
	mat[3] = selected_mat.x[3] * sm::MatrixFix::SCALE_INV;
	mat[4] = selected_mat.x[4] * sm::MatrixFix::TRANSLATE_SCALE_INV;
	mat[5] = selected_mat.x[5] * sm::MatrixFix::TRANSLATE_SCALE_INV;
#else
	memcpy(mat, selected_mat.x, sizeof(selected_mat.x));
#endif // S2_MATRIX_FIX

	auto actor = visitor.GetSelectedActor();
	if (actor) {
		ActorProxy* proxy;
		ActorProxyPool::Instance()->Create(std::const_pointer_cast<s2::Actor>(actor), proxy);
		return proxy;
	}
	return nullptr;
}

extern "C"
int s2_actor_get_count() {
	return -1;
}

extern "C"
void s2_actor_get_aabb(const void* actor, float aabb[4]) {
	const ActorPtr& s2_actor(static_cast<const ActorProxy*>(actor)->actor);
	sm::rect rect;
	if (ProxyHelper::ActorGetAABB(s2_actor.get(), rect)) {
		if (rect.IsValid()) {
			aabb[0] = rect.xmin;
			aabb[1] = rect.ymin;
			aabb[2] = rect.xmax;
			aabb[3] = rect.ymax;
		}
	} else {
		LOGW("s2_actor_get_aabb fail, sym_id %d", s2_actor->GetSprRaw()->GetSymbol()->GetID());
	}
}

extern "C"
void s2_actor_set_pos(void* actor, float x, float y) {
	ActorPtr& s2_actor(static_cast<ActorProxy*>(actor)->actor);
	ProxyHelper::ActorSetPos(s2_actor.get(), sm::vec2(x, y));
}

extern "C"
void s2_actor_get_pos(const void* actor, float* x, float* y) {
	const ActorPtr& s2_actor(static_cast<const ActorProxy*>(actor)->actor);
	sm::vec2 pos;
	if (ProxyHelper::ActorGetPos(s2_actor.get(), pos)) {
		*x = pos.x;
		*y = pos.y;
	} else {
		LOGW("s2_actor_get_pos fail, sym_id %d", s2_actor->GetSprRaw()->GetSymbol()->GetID());
	}
}

extern "C"
void s2_actor_set_angle(void* actor, float angle) {
	ActorPtr& s2_actor(static_cast<ActorProxy*>(actor)->actor);
	ProxyHelper::ActorSetAngle(s2_actor.get(), angle);
}

extern "C"
float s2_actor_get_angle(const void* actor) {
	const ActorPtr& s2_actor(static_cast<const ActorProxy*>(actor)->actor);
	float angle = 0;
	if (ProxyHelper::ActorGetAngle(s2_actor.get(), angle)) {
		return angle;
	} else {
		LOGW("s2_actor_get_angle fail, sym_id %d", s2_actor->GetSprRaw()->GetSymbol()->GetID());
		return 0;
	}
}

extern "C"
void s2_actor_set_scale(void* actor, float sx, float sy) {
	ActorPtr& s2_actor(static_cast<ActorProxy*>(actor)->actor);
	ProxyHelper::ActorSetScale(s2_actor.get(), sm::vec2(sx, sy));
}

extern "C"
void s2_actor_get_scale(const void* actor, float* sx, float* sy) {
	const ActorPtr& s2_actor(static_cast<const ActorProxy*>(actor)->actor);
	sm::vec2 scale;
	if (ProxyHelper::ActorGetScale(s2_actor.get(), scale)) {
		*sx = scale.x;
		*sy = scale.y;
	} else {
		LOGW("s2_actor_get_scale fail, sym_id %d", s2_actor->GetSprRaw()->GetSymbol()->GetID());
	}
}

extern "C"
void s2_actor_get_world_pos(const void* actor, float* x, float* y) {
	const ActorPtr& s2_actor(static_cast<const ActorProxy*>(actor)->actor);
	sm::vec2 pos;
	if (ProxyHelper::ActorGetWorldPos(s2_actor.get(), pos)) {
		*x = pos.x;
		*y = pos.y;
	} else {
		LOGW("s2_actor_get_world_pos fail, sym_id %d", s2_actor->GetSprRaw()->GetSymbol()->GetID());
	}
}

extern "C"
float s2_actor_get_world_angle(const void* actor) {
	const ActorPtr& s2_actor(static_cast<const ActorProxy*>(actor)->actor);
	float angle = 0;
	if (ProxyHelper::ActorGetWorldAngle(s2_actor.get(), angle)) {
		return angle;
	} else {
		LOGW("s2_actor_get_world_angle fail, sym_id %d", s2_actor->GetSprRaw()->GetSymbol()->GetID());
		return 0;
	}
}

extern "C"
void s2_actor_get_world_scale(const void* actor, float* sx, float* sy) {
	const ActorPtr& s2_actor(static_cast<const ActorProxy*>(actor)->actor);
	sm::vec2 scale;
	if (ProxyHelper::ActorGetWorldScale(s2_actor.get(), scale)) {
		*sx = scale.x;
		*sy = scale.y;
	} else {
		LOGW("s2_actor_get_world_scale fail, sym_id %d", s2_actor->GetSprRaw()->GetSymbol()->GetID());
	}
}

extern "C"
void* s2_actor_get_parent(void* actor) {
	ActorPtr& s2_actor(static_cast<ActorProxy*>(actor)->actor);
	auto parent = s2_actor->GetParent();
	if (parent) {
		ActorProxy* proxy;
		ActorProxyPool::Instance()->Create(std::const_pointer_cast<Actor>(parent), proxy);
		return proxy;
	}
	return nullptr;
}

extern "C"
bool s2_actor_get_visible(void* actor) {
	ActorPtr& s2_actor(static_cast<ActorProxy*>(actor)->actor);
	bool visible = true;
	if (ProxyHelper::ActorGetVisible(s2_actor.get(), visible)) {
		return visible;
	} else {
		LOGW("s2_actor_get_visible fail, sym_id %d", s2_actor->GetSprRaw()->GetSymbol()->GetID());
		return true;
	}
}

extern "C"
void s2_actor_set_visible(void* actor, bool visible) {
	ActorPtr& s2_actor(static_cast<ActorProxy*>(actor)->actor);
	ProxyHelper::ActorSetVisible(s2_actor.get(), visible);
}

extern "C"
bool s2_actor_get_editable(void* actor) {
	ActorPtr& s2_actor(static_cast<ActorProxy*>(actor)->actor);
	bool editable = true;
	if (ProxyHelper::ActorGetEditable(s2_actor.get(), editable)) {
		return editable;
	} else {
		LOGW("s2_actor_get_editable fail, sym_id %d", s2_actor->GetSprRaw()->GetSymbol()->GetID());
		return true;
	}
}

extern "C"
void s2_actor_set_editable(void* actor, bool editable) {
	ActorPtr& s2_actor(static_cast<ActorProxy*>(actor)->actor);
	ProxyHelper::ActorSetEditable(s2_actor.get(), editable);
}

extern "C"
uint32_t s2_actor_get_col_mul(void* actor) {
	ActorPtr& s2_actor(static_cast<ActorProxy*>(actor)->actor);
	uint32_t mul;
	if (ProxyHelper::ActorGetColMul(s2_actor.get(), mul)) {
		return mul;
	} else {
		LOGW("s2_actor_get_col_mul fail, sym_id %d", s2_actor->GetSprRaw()->GetSymbol()->GetID());
		return 0xffffffff;
	}
}

extern "C"
void s2_actor_set_col_mul(void* actor, uint32_t abgr) {
	ActorPtr& s2_actor(static_cast<ActorProxy*>(actor)->actor);
	ProxyHelper::ActorSetColMul(s2_actor.get(), abgr);
}

extern "C"
uint32_t s2_actor_get_col_add(void* actor) {
	ActorPtr& s2_actor(static_cast<ActorProxy*>(actor)->actor);
	uint32_t add;
	if (ProxyHelper::ActorGetColAdd(s2_actor.get(), add)) {
		return add;
	} else {
		LOGW("s2_actor_get_col_add fail, sym_id %d", s2_actor->GetSprRaw()->GetSymbol()->GetID());
		return 0;
	}
}

extern "C"
void s2_actor_set_col_add(void* actor, uint32_t abgr) {
	ActorPtr& s2_actor(static_cast<ActorProxy*>(actor)->actor);
	ProxyHelper::ActorSetColAdd(s2_actor.get(), abgr);
}

extern "C"
void s2_actor_get_col_map(void* actor, uint32_t* rmap, uint32_t* gmap, uint32_t* bmap) {
	ActorPtr& s2_actor(static_cast<ActorProxy*>(actor)->actor);
	uint32_t r, g, b;
	if (ProxyHelper::ActorGetColMap(s2_actor.get(), r, g, b)) {
		*rmap = r;
		*gmap = g;
		*bmap = b;
	} else {
		LOGW("s2_actor_get_col_map fail, sym_id %d", s2_actor->GetSprRaw()->GetSymbol()->GetID());
	}
}

extern "C"
void s2_actor_set_col_map(void* actor, uint32_t rmap, uint32_t gmap, uint32_t bmap) {
	ActorPtr& s2_actor(static_cast<ActorProxy*>(actor)->actor);
	ProxyHelper::ActorSetColMap(s2_actor.get(), rmap, gmap, bmap);
}

extern "C"
void s2_actor_set_filter(void* actor, int mode) {
	ActorPtr& s2_actor(static_cast<ActorProxy*>(actor)->actor);
	ProxyHelper::ActorSetFilter(s2_actor.get(), mode);
}

extern "C"
void s2_actor_set_action(void* actor, const char* action) {
	ActorPtr& s2_actor(static_cast<ActorProxy*>(actor)->actor);

	ProxyHelper::ActorSetAction(s2_actor.get(), _char2string(action));
}

extern "C"
bool s2_actor_get_scissor(const void* actor, float* xmin, float* ymin, float* xmax, float* ymax)
{
	const ActorPtr& s2_actor(static_cast<const ActorProxy*>(actor)->actor);
	sm::rect r;
	if (ProxyHelper::ActorGetScissor(s2_actor.get(), r)) {
		*xmin = r.xmin;
		*ymin = r.ymin;
		*xmax = r.xmax;
		*ymax = r.ymax;
		return true;
	} else {
		LOGW("s2_actor_get_scissor fail, sym_id %d", s2_actor->GetSpr()->GetSymbol()->GetID());
		return false;
	}
}

extern "C"
void s2_actor_set_scissor(void* actor, float xmin, float ymin, float xmax, float ymax)
{
	ActorPtr& s2_actor(static_cast<ActorProxy*>(actor)->actor);
	sm::rect r;
	r.xmin = xmin;
	r.ymin = ymin;
	r.xmax = xmax;
	r.ymax = ymax;
	ProxyHelper::ActorSetScissor(s2_actor.get(), r);
}

extern "C"
const char* s2_actor_get_text(void* actor) {
	ActorPtr& s2_actor(static_cast<ActorProxy*>(actor)->actor);
	if (s2_actor->GetSprRaw()->GetSymbol()->Type() != SYM_TEXTBOX) {
		return nullptr;
	} else {
		return std::static_pointer_cast<TextboxActor>(s2_actor)->GetText().c_str();
	}
}

extern "C"
void s2_actor_set_text(void* actor, const char* text) {	
	ActorPtr& s2_actor(static_cast<ActorProxy*>(actor)->actor);
	ProxyHelper::ActorSetText(s2_actor.get(), _char2string(text));
}

extern "C"
bool s2_actor_get_text_size(const void* actor, float* w, float* h) {
	const ActorPtr& s2_actor(static_cast<const ActorProxy*>(actor)->actor);
	if (s2_actor->GetSprRaw()->GetSymbol()->Type() != SYM_TEXTBOX) {
		return false;
	}

	const sm::rect& src = s2_actor->GetAABB().GetRect();
	sm::vec2 min(src.xmin, src.ymin),
		     max(src.xmax, src.ymax);
	S2_MAT mat = s2_actor->GetLocalMat() * s2_actor->GetSprRaw()->GetLocalMat();
	min = mat * min;
	max = mat * max;

	*w = max.x - min.x;
	*h = max.y - min.y;

	return true;
}

extern "C"
void* s2_actor_get_anchor_real_for_fetch(void* actor, bool is_new) {
	ActorPtr& s2_actor(static_cast<ActorProxy*>(actor)->actor);
	if (s2_actor->GetSprRaw()->GetSymbol()->Type() != SYM_ANCHOR) {
		return actor;
	}

	auto anchor = std::static_pointer_cast<AnchorActor>(s2_actor)->GetAnchorPtr();
	if (anchor) {
		if (is_new) {
			ActorProxyPool::Instance()->Delete(s2_actor);
		}
		ActorProxy* proxy;
		ActorProxyPool::Instance()->Create(anchor, proxy);
		return proxy;
	} else {
		return actor;
	}
}

extern "C"
void s2_actor_anchor_deconnect(void* actor) {
	ActorPtr& s2_actor(static_cast<ActorProxy*>(actor)->actor);
	auto parent = s2_actor->GetParent();
	if (parent && parent->GetSprRaw()->GetSymbol()->Type() == SYM_ANCHOR) {
		parent->GetSprRaw()->DelActor(s2_actor);
		s2_actor->SetParent(nullptr);
	}
}

extern "C"
void s2_actor_scale9_resize(void* actor, int w, int h) {
	ActorPtr& s2_actor(static_cast<ActorProxy*>(actor)->actor);
	ProxyHelper::ActorScale9Resize(s2_actor.get(), w, h);
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
			shader->DrawQuad(vertices, texcoords, src_tex_id);
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
#ifndef S2_DISABLE_STATISTICS
	StatPingPong::Instance()->AddCount(StatPingPong::RT_OUTSIDE);
#endif // S2_DISABLE_STATISTICS

	RenderTargetMgr* RT = RenderTargetMgr::Instance();

	RenderScissor::Instance()->Disable();
	RenderCtxStack::Instance()->Push(RenderContext(2, 2, RT->WIDTH, RT->HEIGHT));

	RenderTarget* s2_rt = static_cast<RenderTarget*>(rt);
	s2_rt->Bind();

	_draw(dst, src, src_tex_id);

	s2_rt->Unbind();

	RenderCtxStack::Instance()->Pop();
	RenderScissor::Instance()->Enable();
}

extern "C"
void s2_rt_draw_to(void* rt, const struct s2_region* dst, const struct s2_region* src)
{
#ifndef S2_DISABLE_STATISTICS
	StatPingPong::Instance()->AddCount(StatPingPong::RT_OUTSIDE);
#endif // S2_DISABLE_STATISTICS

	RenderScissor::Instance()->Disable();
	RenderCtxStack::Instance()->Push(RenderContext(2, 2, 0, 0));

	RenderTarget* s2_rt = static_cast<RenderTarget*>(rt);
	int src_tex_id = s2_rt->GetTexID();
	_draw(dst, src, src_tex_id);

	RenderCtxStack::Instance()->Pop();
	RenderScissor::Instance()->Enable();
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

extern "C"
void s2_cam_get(const void* cam, float* x, float* y, float* scale)
{
	const OrthoCamera* o_cam = static_cast<const OrthoCamera*>(cam);
	const sm::vec2& pos = o_cam->GetPosition();
	*x = pos.x;
	*y = pos.y;
	*scale = o_cam->GetScale();
}

extern "C"
void s2_cam_screen2project(const void* cam, int src_x, int src_y, float* dst_x, float* dst_y)
{
	const OrthoCamera* o_cam = static_cast<const OrthoCamera*>(cam);
	const sm::ivec2& sz = Blackboard::Instance()->GetScreenSize();
	sm::vec2 dst = o_cam->TransPosScreenToProject(src_x, src_y, sz.x, sz.y);
	*dst_x = dst.x;
	*dst_y = dst.y;
}

/************************************************************************/
/* rvg                                                                  */
/************************************************************************/

extern "C"
void s2_rvg_set_color(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
	RVG::SetColor(Color(r, g, b, a));
}

extern "C"
void s2_rvg_set_line_width(float width)
{
	RVG::LineWidth(width);
}

extern "C"
void s2_rvg_draw_line(const float* vertices, int count)
{
	RVG::Polyline(vertices, count, false);
}

extern "C"
void s2_rvg_draw_rect(bool filling, float x, float y, float w, float h)
{
	sm::rect r;
	r.xmin = x;
	r.xmax = x + w;
	r.ymin = y;
	r.ymax = y + h;
	RVG::Rect(r, filling);
}

extern "C"
void s2_rvg_draw_circle(bool filling, float cx, float cy, float radius, int segments)
{
	RVG::Circle(sm::vec2(cx, cy), radius, filling, segments);
}

/************************************************************************/
/* other                                                                */
/************************************************************************/

extern "C"
uint32_t s2_trans_color(uint32_t src, enum S2_PIXEL_TYPE src_type, enum S2_PIXEL_TYPE dst_type)
{
	return trans_color(src, PIXEL_TYPE(src_type), PIXEL_TYPE(dst_type));
}

extern "C"
void s2_set_viewport(float x, float y, float w, float h)
{
	RenderContext* ctx = const_cast<RenderContext*>(RenderCtxStack::Instance()->Top());
	ctx->SetViewport(static_cast<int>(x), static_cast<int>(y), static_cast<int>(w), static_cast<int>(h));
}

}
