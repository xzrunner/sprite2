#include "c_wrap_s2.h"

#include "RenderCtxStack.h"
#include "S2_Actor.h"
#include "ActorFactory.h"
#include "S2_Sprite.h"
#include "S2_Symbol.h"
#include "SymType.h"
#include "TextboxActor.h"
#include "PointQueryVisitor.h"
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
#include "StringHelper.h"
#include "sprite2/Statistics.h"

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
#include "ProxySymbol.h"	// todo
#include "ProxyHelper.h"

#include <logger.h>
#include <SM_Math.h>
#include <shaderlab/ShaderMgr.h>
#include <shaderlab/Sprite2Shader.h>
#include <shaderlab/FilterShader.h>

#include <iostream>
#include <stack>

namespace s2
{

static std::string 
_char2string(const char* c_str) 
{
	std::string str;
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
		stack->Push(RenderContext(w, h, w, h));
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

extern "C"
void s2_symbol_draw2(const void* symbol, float x, float y)
{
	const Symbol* s2_sym = static_cast<const Symbol*>(symbol);

	RenderParams rp;
	rp.mt.SetTransformation(x, y, 0, 1, 1, 0, 0, 0, 0);

	DrawNode::Draw(s2_sym, rp);
}

extern "C"
void* s2_symbol_query_child(const void* sym, int child_idx, uint32_t child_id, const char* child_name)
{
	const Symbol* s2_sym = static_cast<const Symbol*>(sym);
	if (s2_sym->Type() != SYM_COMPLEX) {
		return NULL;
	}

	const ComplexSymbol* comp_sym = VI_DOWNCASTING<const ComplexSymbol*>(s2_sym);
	const std::vector<Sprite*>& children = comp_sym->GetAllChildren();
	if (child_idx >= 0 && child_idx < children.size()) {
		Sprite* child = children[child_idx];
		assert(child->GetSymbol()->GetID() == child_id);
		return child;
	} else {
		int child_name_id = SprNameMap::Instance()->QueryID(StringHelper::FromChar(child_name));
		for (int i = 0, n = children.size(); i < n; ++i) {
			Sprite* child = children[i];
			if (child->GetSymbol()->GetID() == child_id &&
				child->GetName() == child_name_id) {
				return child;
			}
		}
		return NULL;
	}
}

extern "C"
int s2_symbol_get_type_id(const char* name)
{
	int ret = SYM_UNKNOWN;
	if (strcmp(name, "IMAGE") == 0) {
		ret = SYM_IMAGE;		
	} else if (strcmp(name, "SCALE9") == 0) {
		ret = SYM_SCALE9;
	} else if (strcmp(name, "ICON") == 0) {
		ret = SYM_ICON;
	} else if (strcmp(name, "TEXTURE") == 0) {
		ret = SYM_TEXTURE;
	} else if (strcmp(name, "TEXTBOX") == 0) {
		ret = SYM_TEXTBOX;
	} else if (strcmp(name, "COMPLEX") == 0) {
		ret = SYM_COMPLEX;
	} else if (strcmp(name, "ANIM") == 0) {
		ret = SYM_ANIMATION;
	} else if (strcmp(name, "ANIM2") == 0) {
		ret = SYM_ANIM2;
	} else if (strcmp(name, "PARTICLE3D") == 0) {
		ret = SYM_PARTICLE3D;
	} else if (strcmp(name, "PARTICLE2D") == 0) {
		ret = SYM_PARTICLE2D;
	} else if (strcmp(name, "SHAPE") == 0) {
		ret = SYM_SHAPE;
	} else if (strcmp(name, "MESH") == 0) {
		ret = SYM_MESH;
	} else if (strcmp(name, "MASK") == 0) {
		ret = SYM_MASK;
	} else if (strcmp(name, "TRAIL") == 0) {
		ret = SYM_TRAIL;
	} else if (strcmp(name, "SKELETON") == 0) {
		ret = SYM_SKELETON;
	} else if (strcmp(name, "MODEL") == 0) {
		ret = SYM_MODEL;
	}
	return ret;
}

/************************************************************************/
/* sprite                                                               */
/************************************************************************/

extern "C"
void s2_spr_draw(const void* actor, float x, float y, float angle, float sx, float sy,
				 float xmin, float ymin, float xmax, float ymax)
{
	RenderParams* rp = RenderParamsPool::Instance()->Pop();
	rp->Reset();

	const Actor* s2_actor = static_cast<const Actor*>(actor);

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

	rp->view_region.xmin = xmin;
	rp->view_region.ymin = ymin;
	rp->view_region.xmax = xmax;
	rp->view_region.ymax = ymax;

	rp->actor = s2_actor;

//	rp->SetDisableCulling(true);

	DrawNode::Draw(s2_actor->GetSpr(), *rp);

	RenderParamsPool::Instance()->Push(rp); 
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
	sm::vec2 pos;
	if (ProxyHelper::SprGetPos(s2_spr, pos)) {
		*x = pos.x;
		*y = pos.y;
	} else {
		LOGW("s2_spr_get_pos fail, sym_id %d", s2_spr->GetSymbol()->GetID());
	}
}

extern "C"
float s2_spr_get_angle(const void* spr) {
	const Sprite* s2_spr = static_cast<const Sprite*>(spr);
	float angle = 0;
	if (ProxyHelper::SprGetAngle(s2_spr, angle)) {
		;
	} else {
		LOGW("s2_spr_get_angle fail, sym_id %d", s2_spr->GetSymbol()->GetID());
	}
	return angle;
}

extern "C"
void s2_spr_get_scale(const void* spr, float* sx, float* sy) {
	const Sprite* s2_spr = static_cast<const Sprite*>(spr);
	sm::vec2 scale;
	if (ProxyHelper::SprGetScale(s2_spr, scale)) {
		*sx = scale.x;
		*sy = scale.y;
	} else {
		LOGW("s2_spr_get_scale fail, sym_id %d", s2_spr->GetSymbol()->GetID());
	}
}

extern "C"
void s2_spr_set_col_mul(void* spr, uint32_t abgr) {
	Sprite* s2_spr = static_cast<Sprite*>(spr);
	RenderColor rc = s2_spr->GetColor();
	rc.SetMulABGR(abgr);
	s2_spr->SetColor(rc);
}

extern "C"
void s2_spr_set_col_add(void* spr, uint32_t abgr) {
	Sprite* s2_spr = static_cast<Sprite*>(spr);
	RenderColor rc = s2_spr->GetColor();
	rc.SetAddABGR(abgr);
	s2_spr->SetColor(rc);
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
	std::string name;
	SprNameMap::Instance()->IDToStr(static_cast<Sprite*>(spr)->GetName(), name);
	return name.c_str();
}

extern "C"
int s2_spr_get_sym_id(const void* spr) {
	const Sprite* s2_spr = static_cast<const Sprite*>(spr);
	int id = 0;
	if (ProxyHelper::SprGetSymID(s2_spr, id)) {
		;
	} else {
		LOGW("s2_spr_get_sym_id fail, sym_id %d", s2_spr->GetSymbol()->GetID());
	}
	return id;
}

extern "C"
int s2_spr_get_sym_type(const void* spr) {
 	const Sprite* s2_spr = static_cast<const Sprite*>(spr);
	return s2_spr->GetSymbol()->Type();
}

extern "C"
void s2_spr_set_downsample(void* spr, float downsample) {
	Sprite* s2_spr = static_cast<Sprite*>(spr);
	s2_spr->SetDownsample(downsample);
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
	memcpy(outer.x, mat, sizeof(outer.x));
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
	return ProxyHelper::SprPointTest(s2_spr, sm::vec2(x, y));
}

extern "C"
void* s2_spr_point_query(const void* spr, float x, float y, float mat[6]) {
	const Sprite* s2_spr = static_cast<const Sprite*>(spr);
	return const_cast<Actor*>(ProxyHelper::SprPointQuery(s2_spr, sm::vec2(x, y), mat));
}

extern "C"
bool s2_spr_get_force_update(const void* spr) {
	const Sprite* s2_spr = static_cast<const Sprite*>(spr);
	bool force = false;
	if (ProxyHelper::SprGetForceUpdate(s2_spr, force)) {
		return force;
	} else {
		LOGW("s2_spr_get_force_update fail, sym_id %d", s2_spr->GetSymbol()->GetID());
		return false;		
	}
}

extern "C"
void s2_spr_set_force_update(void* spr, bool force) {
	Sprite* s2_spr = static_cast<Sprite*>(spr);
	ProxyHelper::SprSetForceUpdate(s2_spr, force);
}

extern "C"
int s2_spr_get_frame_count(void* spr) {
	Sprite* s2_spr = static_cast<Sprite*>(spr);
	int count = -1;
	if (ProxyHelper::SprGetFrameCount(s2_spr, count)) {
		;
	} else {
		LOGW("s2_spr_get_frame_count fail, sym_id %d", s2_spr->GetSymbol()->GetID());
	}
	return count;
}

extern "C"
bool s2_spr_has_action(const void* spr, const char* name) {
	const Sprite* s2_spr = static_cast<const Sprite*>(spr);
	return ProxyHelper::SprHasAction(s2_spr, _char2string(name));
}

extern "C"
bool s2_spr_get_scissor(const void* spr, float* xmin, float* ymin, float* xmax, float* ymax)
{
	const Sprite* s2_spr = static_cast<const Sprite*>(spr);
	sm::rect r;
	if (ProxyHelper::SprGetScissor(s2_spr, r)) {
		*xmin = r.xmin;
		*ymin = r.ymin;
		*xmax = r.xmax;
		*ymax = r.ymax;
		return true;
	} else {
		LOGW("s2_spr_get_scissor fail, sym_id %d", s2_spr->GetSymbol()->GetID());
		return false;
	}
}

extern "C"
void s2_spr_set_scissor(void* spr, float xmin, float ymin, float xmax, float ymax)
{
	Sprite* s2_spr = static_cast<Sprite*>(spr);
	sm::rect r;
	r.xmin = xmin;
	r.ymin = ymin;
	r.xmax = xmax;
	r.ymax = ymax;
	ProxyHelper::SprSetScissor(s2_spr, r);
}

extern "C"
void s2_spr_textbox_reset_time(void* spr)
{
	Sprite* s2_spr = static_cast<Sprite*>(spr);
	ProxyHelper::SprTextboxResetTime(s2_spr);
}

extern "C"
void s2_spr_anim2_set_static_time(void* spr, int time) 
{
	Sprite* s2_spr = static_cast<Sprite*>(spr);
	ProxyHelper::SprAnim2SetStaticTime(s2_spr, time);
}

extern "C"
void s2_spr_p3d_set_local(void* spr, bool local) 
{
	Sprite* s2_spr = static_cast<Sprite*>(spr);
	ProxyHelper::SprP3dSetLocal(s2_spr, local);
}

extern "C"
void s2_spr_p3d_set_loop(void* spr, bool loop)
{
	Sprite* s2_spr = static_cast<Sprite*>(spr);
	ProxyHelper::SprP3dSetLoop(s2_spr, loop);
}

extern "C"
bool s2_spr_p3d_is_finished(const void* spr) 
{
	const Sprite* s2_spr = static_cast<const Sprite*>(spr);
	return ProxyHelper::SprP3dIsFinished(s2_spr);
}

extern "C"
void s2_spr_p3d_update(void* spr, float dt)
{
	Sprite* s2_spr = static_cast<Sprite*>(spr);
	ProxyHelper::SprP3dUpdate(s2_spr, dt);
}

extern "C"
void s2_spr_p3d_buffer_draw(float x, float y, float scale)
{
	Particle3d::Instance()->BufferDraw(x, y, scale);
}

extern "C"
void s2_spr_proxy_get_children(const void* spr, void* children[], int children_cap, int* count)
{
	const Sprite* s2_spr = static_cast<const Sprite*>(spr);

	std::vector<const Actor*> actors;
	ProxyHelper::SprGetProxyChildren(s2_spr, actors);
	int n = std::min(children_cap, static_cast<int>(actors.size()));
	for (int i = 0; i < n; ++i) {
		children[i] = const_cast<Actor*>(actors[i]);
	}
	*count = n;
}

extern "C"
void s2_spr_set_dtex_enable(void* spr, bool enable)
{
	Sprite* s2_spr = static_cast<Sprite*>(spr);
	ProxyHelper::SetDTexEnable(s2_spr, enable);
}

extern "C"
void s2_spr_set_dtex_force_cached(void* spr, bool cache)
{
	Sprite* s2_spr = static_cast<Sprite*>(spr);
	ProxyHelper::SetDTexForceCached(s2_spr, cache);
}

extern "C"
void s2_spr_set_dtex_force_cached_dirty(void* spr, bool dirty)
{
	Sprite* s2_spr = static_cast<Sprite*>(spr);
	ProxyHelper::SetDTexForceCachedDirty(s2_spr, dirty);
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

	RenderParams* rp_child = RenderParamsPool::Instance()->Pop();
	*rp_child = rp;
	while (path.size() > 1) {
		const Actor* curr = path.top();
		path.pop();
		rp.actor = curr;
		DrawNode::Prepare(rp, curr->GetSpr(), *rp_child);
		rp = *rp_child;
	}

	S2_MAT mt;
	mt.SetTransformation(x, y, angle, sx, sy, 0, 0, 0, 0);

	rp.actor = s2_actor;
	rp.mt = mt * rp.mt;
	DrawNode::Draw(s2_spr, rp);

	RenderParamsPool::Instance()->Push(rp_child);
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
void s2_actor_update(void* actor, bool force) {
	const Actor* s2_actor = static_cast<const Actor*>(actor);
	const Sprite* s2_spr = s2_actor->GetSpr();

	UpdateParams up(s2_actor);
	up.SetPrevMat(get_actor_world_mat(s2_actor->GetParent()));
	up.SetForce(force);

	int old_inherit_update = ProxyHelper::SprGetInheritUpdate(s2_spr);
	if (old_inherit_update != -1) {
		ProxyHelper::SprSetInheritUpdate(s2_spr, true);
		const_cast<Sprite*>(s2_spr)->Update(up);
		ProxyHelper::SprSetInheritUpdate(s2_spr, old_inherit_update == 1);
	} else {
		const_cast<Sprite*>(s2_spr)->Update(up);
	}
}

extern "C"
void s2_actor_msg_start(void* actor) {
	const Actor* s2_actor = static_cast<const Actor*>(actor);
	const Sprite* s2_spr = s2_actor->GetSpr();
	UpdateParams up(s2_actor);
	up.SetPrevMat(get_actor_world_mat(s2_actor->GetParent()));
	const_cast<Sprite*>(s2_spr)->OnMessage(up, MSG_START);
}

extern "C"
void s2_actor_msg_stop(void* actor) {
	const Actor* s2_actor = static_cast<const Actor*>(actor);
	const Sprite* s2_spr = s2_actor->GetSpr();
	UpdateParams up(s2_actor);
	up.SetPrevMat(get_actor_world_mat(s2_actor->GetParent()));
	const_cast<Sprite*>(s2_spr)->OnMessage(up, MSG_STOP);
}

extern "C"
void s2_actor_msg_clear(void* actor) {
	const Actor* s2_actor = static_cast<const Actor*>(actor);
	const Sprite* s2_spr = s2_actor->GetSpr();
	UpdateParams up(s2_actor);
	up.SetPrevMat(get_actor_world_mat(s2_actor->GetParent()));
	const_cast<Sprite*>(s2_spr)->OnMessage(up, MSG_CLEAR);
}

extern "C"
void s2_actor_set_frame(void* actor, int frame) {
	Actor* s2_actor = static_cast<Actor*>(actor);
	ProxyHelper::ActorSetFrame(s2_actor, frame);
}

extern "C"
int s2_actor_get_frame(void* actor) {
	const Actor* s2_actor = static_cast<const Actor*>(actor);
	int frame = -1;
	if (ProxyHelper::ActorGetFrame(s2_actor, frame)) {
		return frame;
	} else {
		LOGW("s2_actor_get_frame fail, sym_id %d", s2_actor->GetSpr()->GetSymbol()->GetID());
		return -1;
	}
}

extern "C"
int s2_actor_get_component_count(void* actor) {
	const Actor* s2_actor = static_cast<const Actor*>(actor);
	int count = -1;
	if (ProxyHelper::ActorGetComponentCount(s2_actor, count)) {
		return count;
	} else {
		LOGW("s2_actor_get_component_count fail, sym_id %d", s2_actor->GetSpr()->GetSymbol()->GetID());
		return -1;
	}
}

extern "C"
void* s2_actor_fetch_child(const void* actor, const char* name) {
	const Actor* s2_actor = static_cast<const Actor*>(actor);
	int name_id = SprNameMap::Instance()->StrToID(StringHelper::FromChar(name));
	const Sprite* child = s2_actor->GetSpr()->FetchChildByName(name_id, s2_actor);
	if (child) {
		return const_cast<Sprite*>(child);
	} else {
		return NULL;
	}
}

extern "C"
void* s2_actor_fetch_child_by_index(const void* actor, int idx) {
	const Actor* s2_actor = static_cast<const Actor*>(actor);
	const Sprite* child = s2_actor->GetSpr()->FetchChildByIdx(idx, s2_actor);
	if (child) {
		return const_cast<Sprite*>(child);
	} else {
		return NULL;
	}
}

static int
_actor_mount(const Actor* parent, Sprite* old_child, const Actor* new_child) {
	if (!old_child) {
		return -1;		
	}
	int sym_type = old_child->GetSymbol()->Type();
	if (sym_type == SYM_PROXY) {
		const ProxySymbol* proxy_sym = VI_DOWNCASTING<const ProxySymbol*>(old_child->GetSymbol());
		const std::vector<std::pair<const Actor*, Sprite*> >& items = proxy_sym->GetItems();
		int ret = 0;
		for (int i = 0, n = items.size(); i < n; ++i) 
		{
			int _ret = _actor_mount(items[i].first, items[i].second, new_child);
			if (_ret != 0) {
				ret = _ret;
			}
		}
		return ret;
	} else if (sym_type == SYM_ANCHOR) {
		AnchorSprite* anchor_spr = VI_DOWNCASTING<AnchorSprite*>(old_child);
		anchor_spr->AddAnchor(new_child, parent);
		return 0;
	} else {
		return -2;
	}
}

// ret: 0 ok, -1 no child with name, -2 child isn't anchor
extern "C"
int s2_actor_mount(const void* parent, const char* name, const void* child) {
	const Actor* p_actor = static_cast<const Actor*>(parent);
	const Sprite* p_spr = p_actor->GetSpr();
	const Actor* new_actor = static_cast<const Actor*>(child);
	int name_id = SprNameMap::Instance()->StrToID(StringHelper::FromChar(name));
	Sprite* old_spr = p_spr->FetchChildByName(name_id, p_actor);
	if (!old_spr) {
		return -1;		
	}

	int sym_type = old_spr->GetSymbol()->Type();
	if (sym_type == SYM_PROXY) {
		const ProxySymbol* proxy_sym = VI_DOWNCASTING<const ProxySymbol*>(old_spr->GetSymbol());
		const std::vector<std::pair<const Actor*, Sprite*> >& items = proxy_sym->GetItems();
		int ret = 0;
		for (int i = 0, n = items.size(); i < n; ++i) 
		{
			int _ret = _actor_mount(items[i].first, items[i].second, new_actor);
			if (_ret != 0) {
				ret = _ret;
			}
		}
		old_spr->RemoveReference();
		return ret;
	} else if (sym_type == SYM_ANCHOR) {
		return _actor_mount(p_actor, old_spr, new_actor);
	} else {
		return -2;
	}
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
	memcpy(mat, selected_mat.x, sizeof(selected_mat.x));
#endif // S2_MATRIX_FIX

	return const_cast<Actor*>(visitor.GetSelectedActor());
}

extern "C"
void* s2_actor_create(const void* parent_actor, void* child_spr) {
	const Actor* parent = static_cast<const Actor*>(parent_actor);
	Sprite* child = static_cast<Sprite*>(child_spr);
	return ActorFactory::Instance()->Create(parent, child);
}

extern "C"
int s2_actor_get_count() {
	return -1;
}

extern "C"
void* s2_actor_get_spr(void* actor) {
	return const_cast<Sprite*>(static_cast<Actor*>(actor)->GetSpr());
}

extern "C"
void s2_actor_get_aabb(const void* actor, float aabb[4]) {
	const Actor* s2_actor = static_cast<const Actor*>(actor);
	sm::rect rect;
	if (ProxyHelper::ActorGetAABB(s2_actor, rect)) {
		if (rect.IsValid()) {
			aabb[0] = rect.xmin;
			aabb[1] = rect.ymin;
			aabb[2] = rect.xmax;
			aabb[3] = rect.ymax;
		}
	} else {
		LOGW("s2_actor_get_aabb fail, sym_id %d", s2_actor->GetSpr()->GetSymbol()->GetID());
	}
}

extern "C"
void s2_actor_set_pos(void* actor, float x, float y) {
	Actor* s2_actor = static_cast<Actor*>(actor);
	ProxyHelper::ActorSetPos(s2_actor, sm::vec2(x, y));
}

extern "C"
void s2_actor_get_pos(const void* actor, float* x, float* y) {
	const Actor* s2_actor = static_cast<const Actor*>(actor);
	sm::vec2 pos;
	if (ProxyHelper::ActorGetPos(s2_actor, pos)) {
		*x = pos.x;
		*y = pos.y;
	} else {
		LOGW("s2_actor_get_pos fail, sym_id %d", s2_actor->GetSpr()->GetSymbol()->GetID());
	}
}

extern "C"
void s2_actor_set_angle(void* actor, float angle) {
	Actor* s2_actor = static_cast<Actor*>(actor);
	ProxyHelper::ActorSetAngle(s2_actor, angle);
}

extern "C"
float s2_actor_get_angle(const void* actor) {
	const Actor* s2_actor = static_cast<const Actor*>(actor);
	float angle = 0;
	if (ProxyHelper::ActorGetAngle(s2_actor, angle)) {
		return angle;
	} else {
		LOGW("s2_actor_get_angle fail, sym_id %d", s2_actor->GetSpr()->GetSymbol()->GetID());
		return 0;
	}
}

extern "C"
void s2_actor_set_scale(void* actor, float sx, float sy) {
	Actor* s2_actor = static_cast<Actor*>(actor);
	ProxyHelper::ActorSetScale(s2_actor, sm::vec2(sx, sy));
}

extern "C"
void s2_actor_get_scale(const void* actor, float* sx, float* sy) {
	const Actor* s2_actor = static_cast<const Actor*>(actor);
	sm::vec2 scale;
	if (ProxyHelper::ActorGetScale(s2_actor, scale)) {
		*sx = scale.x;
		*sy = scale.y;
	} else {
		LOGW("s2_actor_get_scale fail, sym_id %d", s2_actor->GetSpr()->GetSymbol()->GetID());
	}
}

extern "C"
void s2_actor_get_world_pos(const void* actor, float* x, float* y) {
	const Actor* s2_actor = static_cast<const Actor*>(actor);
	sm::vec2 pos;
	if (ProxyHelper::ActorGetWorldPos(s2_actor, pos)) {
		*x = pos.x;
		*y = pos.y;
	} else {
		LOGW("s2_actor_get_world_pos fail, sym_id %d", s2_actor->GetSpr()->GetSymbol()->GetID());
	}
}

extern "C"
float s2_actor_get_world_angle(const void* actor) {
	const Actor* s2_actor = static_cast<const Actor*>(actor);
	float angle = 0;
	if (ProxyHelper::ActorGetWorldAngle(s2_actor, angle)) {
		return angle;
	} else {
		LOGW("s2_actor_get_world_angle fail, sym_id %d", s2_actor->GetSpr()->GetSymbol()->GetID());
		return 0;
	}
}

extern "C"
void s2_actor_get_world_scale(const void* actor, float* sx, float* sy) {
	const Actor* s2_actor = static_cast<const Actor*>(actor);
	sm::vec2 scale;
	if (ProxyHelper::ActorGetWorldScale(s2_actor, scale)) {
		*sx = scale.x;
		*sy = scale.y;
	} else {
		LOGW("s2_actor_get_world_scale fail, sym_id %d", s2_actor->GetSpr()->GetSymbol()->GetID());
	}
}

extern "C"
void* s2_actor_get_parent(void* actor) {
	Actor* s2_actor = static_cast<Actor*>(actor);
	return const_cast<Actor*>(s2_actor->GetParent());
}

extern "C"
bool s2_actor_get_visible(void* actor) {
	Actor* s2_actor = static_cast<Actor*>(actor);
	bool visible = true;
	if (ProxyHelper::ActorGetVisible(s2_actor, visible)) {
		return visible;
	} else {
		LOGW("s2_actor_get_visible fail, sym_id %d", s2_actor->GetSpr()->GetSymbol()->GetID());
		return true;
	}
}

extern "C"
void s2_actor_set_visible(void* actor, bool visible) {
	Actor* s2_actor = static_cast<Actor*>(actor);
	ProxyHelper::ActorSetVisible(s2_actor, visible);
}

extern "C"
bool s2_actor_get_editable(void* actor) {
	Actor* s2_actor = static_cast<Actor*>(actor);
	bool editable = true;
	if (ProxyHelper::ActorGetEditable(s2_actor, editable)) {
		return editable;
	} else {
		LOGW("s2_actor_get_editable fail, sym_id %d", s2_actor->GetSpr()->GetSymbol()->GetID());
		return true;
	}
}

extern "C"
void s2_actor_set_editable(void* actor, bool editable) {
	Actor* s2_actor = static_cast<Actor*>(actor);
	ProxyHelper::ActorSetEditable(s2_actor, editable);
}

extern "C"
uint32_t s2_actor_get_col_mul(void* actor) {
	Actor* s2_actor = static_cast<Actor*>(actor);
	uint32_t mul;
	if (ProxyHelper::ActorGetColMul(s2_actor, mul)) {
		return mul;
	} else {
		LOGW("s2_actor_get_col_mul fail, sym_id %d", s2_actor->GetSpr()->GetSymbol()->GetID());
		return 0xffffffff;
	}
}

extern "C"
void s2_actor_set_col_mul(void* actor, uint32_t abgr) {
	Actor* s2_actor = static_cast<Actor*>(actor);
	ProxyHelper::ActorSetColMul(s2_actor, abgr);
}

extern "C"
uint32_t s2_actor_get_col_add(void* actor) {
	Actor* s2_actor = static_cast<Actor*>(actor);
	uint32_t add;
	if (ProxyHelper::ActorGetColAdd(s2_actor, add)) {
		return add;
	} else {
		LOGW("s2_actor_get_col_add fail, sym_id %d", s2_actor->GetSpr()->GetSymbol()->GetID());
		return 0;
	}
}

extern "C"
void s2_actor_set_col_add(void* actor, uint32_t abgr) {
	Actor* s2_actor = static_cast<Actor*>(actor);
	ProxyHelper::ActorSetColAdd(s2_actor, abgr);
}

extern "C"
void s2_actor_get_col_map(void* actor, uint32_t* rmap, uint32_t* gmap, uint32_t* bmap) {
	Actor* s2_actor = static_cast<Actor*>(actor);
	uint32_t r, g, b;
	if (ProxyHelper::ActorGetColMap(s2_actor, r, g, b)) {
		*rmap = r;
		*gmap = g;
		*bmap = b;
	} else {
		LOGW("s2_actor_get_col_map fail, sym_id %d", s2_actor->GetSpr()->GetSymbol()->GetID());
	}
}

extern "C"
void s2_actor_set_col_map(void* actor, uint32_t rmap, uint32_t gmap, uint32_t bmap) {
	Actor* s2_actor = static_cast<Actor*>(actor);
	ProxyHelper::ActorSetColMap(s2_actor, rmap, gmap, bmap);
}

extern "C"
void s2_actor_set_filter(void* actor, int mode) {
	Actor* s2_actor = static_cast<Actor*>(actor);
	ProxyHelper::ActorSetFilter(s2_actor, mode);
}

extern "C"
void s2_actor_set_action(void* actor, const char* action) {
	Actor* s2_actor = static_cast<Actor*>(actor);
	ProxyHelper::ActorSetAction(s2_actor, _char2string(action));
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
	ProxyHelper::ActorSetText(s2_actor, _char2string(text));
}

extern "C"
bool s2_actor_get_text_size(const void* actor, float* w, float* h) {
	const Actor* s2_actor = static_cast<const Actor*>(actor);
	if (s2_actor->GetSpr()->GetSymbol()->Type() != SYM_TEXTBOX) {
		return false;
	}

	const sm::rect& src = s2_actor->GetAABB().GetRect();
	sm::vec2 min(src.xmin, src.ymin),
		     max(src.xmax, src.ymax);
	S2_MAT mat = s2_actor->GetLocalMat() * s2_actor->GetSpr()->GetLocalMat();
	min = mat * min;
	max = mat * max;

	*w = max.x - min.x;
	*h = max.y - min.y;

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
void s2_actor_anchor_deconnect(void* actor) {
	Actor* s2_actor = static_cast<Actor*>(actor);
	const Actor* parent = s2_actor->GetParent();
	if (parent && parent->GetSpr()->GetSymbol()->Type() == SYM_ANCHOR) {
		parent->GetSpr()->DelActor(s2_actor);
		s2_actor->SetParent(NULL);
	}
}

extern "C"
void s2_actor_scale9_resize(void* actor, int w, int h) {
	Actor* s2_actor = static_cast<Actor*>(actor);
	ProxyHelper::ActorScale9Resize(s2_actor, w, h);
}

extern "C"
void s2_actor_build_flatten(void* actor) {
	Actor* s2_actor = static_cast<Actor*>(actor);
	const Symbol* s2_sym = s2_actor->GetSpr()->GetSymbol();
	if (s2_sym->Type() == SYM_COMPLEX) {
		VI_DOWNCASTING<const ComplexSymbol*>(s2_sym)->BuildFlatten(s2_actor);
	} else if (s2_sym->Type() == SYM_ANIMATION) {
		VI_DOWNCASTING<const AnimSymbol*>(s2_sym)->BuildFlatten(s2_actor);
	}
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
	Statistics::Instance()->AddRTOutside();

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
	Statistics::Instance()->AddRTOutside();

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
void s2_stat_get_pingpong_count(struct s2_stat_pingpong_count* dst)
{
	const Statistics::PingPongCount& src = Statistics::Instance()->GetPingPongCount();
	memcpy(dst, &src, sizeof(*dst));
}

extern "C"
void s2_stat_reset()
{
	Statistics::Instance()->Reset();
}

}
