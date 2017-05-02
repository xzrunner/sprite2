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
#include "SetStaticFrameVisitor.h"

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
#include "ProxySymbol.h"

#include <logger.h>
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

/************************************************************************/
/* sprite                                                               */
/************************************************************************/

static RenderParams rp;

extern "C"
void s2_spr_draw(const void* actor, float x, float y, float angle, float sx, float sy,
				 float xmin, float ymin, float xmax, float ymax)
{
	const Actor* s2_actor = static_cast<const Actor*>(actor);

	float* m = rp.mt.x;
	float c = sm::cos(angle), s = sm::sin(angle);
	m[0] = c * sx;
	m[1] = s * sx;
	m[2] = - s * sy;
	m[3] = c * sy;
	m[4] = x;
	m[5] = y;

	rp.view_region.xmin = xmin;
	rp.view_region.ymin = ymin;
	rp.view_region.xmax = xmax;
	rp.view_region.ymax = ymax;

	rp.actor = s2_actor;

	DrawNode::Draw(s2_actor->GetSpr(), rp, false);
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
	if (s2_spr->GetSymbol()->Type() == SYM_PROXY) {
		LOGW("%s", "no use, s2_spr_get_pos for proxy");
	}
	*x = s2_spr->GetPosition().x;
	*y = s2_spr->GetPosition().y;
}

extern "C"
float s2_spr_get_angle(const void* spr) {
	const Sprite* s2_spr = static_cast<const Sprite*>(spr);
	if (s2_spr->GetSymbol()->Type() == SYM_PROXY) {
		LOGW("%s", "no use, s2_spr_get_pos for proxy");
	}
	return s2_spr->GetAngle();
}

extern "C"
void s2_spr_get_scale(const void* spr, float* sx, float* sy) {
	const Sprite* s2_spr = static_cast<const Sprite*>(spr);
	if (s2_spr->GetSymbol()->Type() == SYM_PROXY) {
		LOGW("%s", "no use, s2_spr_get_pos for proxy");
	}
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

static 
bool _set_action(Actor* actor, const char* action) {
	if (actor->GetSpr()->GetSymbol()->Type() != SYM_COMPLEX) {
		return false;
	}

	const ComplexSymbol* sym_complex = VI_DOWNCASTING<const ComplexSymbol*>(actor->GetSpr()->GetSymbol());
	ComplexActor* actor_complex = static_cast<ComplexActor*>(actor);
	int action_idx = -1;
	if (action) {
		action_idx = sym_complex->GetActionIdx(action);
	}
	actor_complex->SetAction(action_idx);

	s2_actor_msg_start(actor);

	return true;
}

extern "C"
void s2_spr_set_action(void* actor, const char* action) {
	Actor* s2_actor = static_cast<Actor*>(actor);
	bool succ = false;
	int type = s2_actor->GetSpr()->GetSymbol()->Type();
	if (type == SYM_PROXY)
	{
		const ProxySymbol* proxy_sym = VI_DOWNCASTING<const ProxySymbol*>(s2_actor->GetSpr()->GetSymbol());
		const std::vector<std::pair<const Actor*, Sprite*> >& items = proxy_sym->GetItems();
		for (int i = 0, n = items.size(); i < n; ++i) {
			const Actor* child_actor = items[i].second->QueryActor(items[i].first);
			if (_set_action(const_cast<Actor*>(child_actor), action)) {
				succ = true;
			}
		}
	}
	else
	{
		succ = _set_action(s2_actor, action);
	}
	if (!succ) {
		LOGW("s2_spr_set_action fail. spr %d, type %d\n", s2_actor->GetSpr()->GetSymbol()->GetID(), type);
	}
}

extern "C"
int s2_spr_get_frame_count(void* spr) {
	Sprite* s2_spr = static_cast<Sprite*>(spr);
	if (s2_spr->GetSymbol()->Type() == SYM_ANIMATION) {
		AnimSprite* anim = VI_DOWNCASTING<AnimSprite*>(s2_spr);
		const AnimSymbol* sym = VI_DOWNCASTING<const AnimSymbol*>(anim->GetSymbol());
		return sym->GetMaxFrameIdx();
	} else {
		LOGW("no use, s2_spr_get_frame_count: not animation spr %d", s2_spr->GetSymbol()->GetID());
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
	sm::vec2 pos(x, y);
	if (s2_spr->GetSymbol()->Type() == SYM_PROXY) {
		const ProxySymbol* proxy_sym = VI_DOWNCASTING<const ProxySymbol*>(s2_spr->GetSymbol());
		const std::vector<std::pair<const Actor*, Sprite*> >& items = proxy_sym->GetItems();
		for (int i = 0, n = items.size(); i < n; ++i) {
			if (items[i].second->GetBounding()->IsContain(pos)) {
				return true;
			}
		}
		return false;
	} else {
		return s2_spr->GetBounding()->IsContain(pos);
	}
}

static void*
_point_query(const Sprite* spr, const sm::vec2& pos, float mat[6]) {
	PointQueryVisitor visitor(pos);

	SprVisitorParams params;
	spr->Traverse(visitor, params);
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
	memcpy(mat, selected_mat.x, sizeof(selected_mat.x));
#endif // S2_MATRIX_FIX

	return const_cast<Actor*>(ret);
}

extern "C"
void* s2_spr_point_query(const void* spr, float x, float y, float mat[6]) {
	const Sprite* s2_spr = static_cast<const Sprite*>(spr);
	sm::vec2 pos(x, y);
	if (s2_spr->GetSymbol()->Type() == SYM_PROXY) {
		const ProxySymbol* proxy_sym = VI_DOWNCASTING<const ProxySymbol*>(s2_spr->GetSymbol());
		const std::vector<std::pair<const Actor*, Sprite*> >& items = proxy_sym->GetItems();
		for (int i = 0, n = items.size(); i < n; ++i) {
			void* ret = _point_query(items[i].second, pos, mat);
			if (ret) {
				return ret;
			}
		}
		return NULL;
	} else {
		return _point_query(s2_spr, pos, mat);
	}
}

static bool
_has_action(const Sprite* spr, const char* name) {
	if (spr->GetSymbol()->Type() != SYM_COMPLEX) {
		return false;
	}

	const ComplexSymbol* sym = static_cast<const ComplexSymbol*>(spr->GetSymbol());
	const std::vector<ComplexSymbol::Action>& actions = sym->GetActions();
	for (int i = 0, n = actions.size(); i < n; ++i) {
		if (name == actions[i].name) {
			return true;
		}
	}
	return false;
}

extern "C"
bool s2_spr_has_action(const void* spr, const char* name) {
	const Sprite* s2_spr = static_cast<const Sprite*>(spr);
	if (s2_spr->GetSymbol()->Type() == SYM_PROXY) 
	{
		const ProxySymbol* proxy_sym = VI_DOWNCASTING<const ProxySymbol*>(s2_spr->GetSymbol());
		const std::vector<std::pair<const Actor*, Sprite*> >& items = proxy_sym->GetItems();
		for (int i = 0, n = items.size(); i < n; ++i) {
			if (_has_action(items[i].second, name)) {
				return true;
			}
		}
		return false;
	} 
	else 
	{
		return _has_action(s2_spr, name);
	}
}

extern "C"
bool s2_spr_get_scissor(const void* spr, float* xmin, float* ymin, float* xmax, float* ymax)
{
	const Sprite* s2_spr = static_cast<const Sprite*>(spr);
	int type = s2_spr->GetSymbol()->Type();
	if (type != SYM_COMPLEX) {
		*xmin = *ymin = *xmax = *ymax = 0;
		LOGW("no use, s2_spr_get_scissor for spr %d, type %d\n", s2_spr->GetSymbol()->GetID(), type);
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

static bool
_set_scissor(Sprite* spr, float xmin, float ymin, float xmax, float ymax) {
	if (spr->GetSymbol()->Type() != SYM_COMPLEX) {
		return false;
	}

	const ComplexSprite* complex_spr = static_cast<const ComplexSprite*>(spr);
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
bool s2_spr_set_scissor(void* spr, float xmin, float ymin, float xmax, float ymax)
{
	Sprite* s2_spr = static_cast<Sprite*>(spr);
	if (s2_spr->GetSymbol()->Type() == SYM_PROXY) 
	{
		const ProxySymbol* proxy_sym = VI_DOWNCASTING<const ProxySymbol*>(s2_spr->GetSymbol());
		const std::vector<std::pair<const Actor*, Sprite*> >& items = proxy_sym->GetItems();
		bool ret = false;
		for (int i = 0, n = items.size(); i < n; ++i) {
			if (_set_scissor(items[i].second, xmin, ymin, xmax, ymax)) {
				ret = true;
			}
		}
		return ret;
	} 
	else
	{
		return _set_scissor(s2_spr, xmin, ymin, xmax, ymax);
	}
}

static bool
_textbox_reset_time(Sprite* spr)
{
	if (spr->GetSymbol()->Type() != SYM_TEXTBOX) {
		return false;
	}

	TextboxSprite* tb_spr = static_cast<TextboxSprite*>(spr);
	tb_spr->ResetTime();

	return true;
}

extern "C"
void s2_spr_textbox_reset_time(void* spr)
{
	Sprite* s2_spr = static_cast<Sprite*>(spr);
	if (s2_spr->GetSymbol()->Type() == SYM_PROXY) {
		const ProxySymbol* proxy_sym = VI_DOWNCASTING<const ProxySymbol*>(s2_spr->GetSymbol());
		const std::vector<std::pair<const Actor*, Sprite*> >& items = proxy_sym->GetItems();
		for (int i = 0, n = items.size(); i < n; ++i) {
			_textbox_reset_time(items[i].second);
		}
	} else {
		_textbox_reset_time(s2_spr);
	}
}

static bool
_set_static_time(Sprite* spr, int time)
{
	if (spr->GetSymbol()->Type() != SYM_ANIM2) {
		return false;
	}

	Anim2Sprite* a2_spr = static_cast<Anim2Sprite*>(spr);
	a2_spr->SetStaticTime(time);

	return true;
}

extern "C"
void s2_spr_anim2_set_static_time(void* spr, int time) 
{
	Sprite* s2_spr = static_cast<Sprite*>(spr);
	if (s2_spr->GetSymbol()->Type() == SYM_PROXY) {
		const ProxySymbol* proxy_sym = VI_DOWNCASTING<const ProxySymbol*>(s2_spr->GetSymbol());
		const std::vector<std::pair<const Actor*, Sprite*> >& items = proxy_sym->GetItems();
		for (int i = 0, n = items.size(); i < n; ++i) {
			_set_static_time(items[i].second, time);
		}
	} else {
		_set_static_time(s2_spr, time);
	}
}

static bool
_p3d_set_local(Sprite* spr, bool local)
{
	if (spr->GetSymbol()->Type() != SYM_PARTICLE3D) {
		return false;
	}

	Particle3dSprite* p3d_spr = static_cast<Particle3dSprite*>(spr);
	p3d_spr->SetLocal(local);

	return true;
}

extern "C"
void s2_spr_p3d_set_local(void* spr, bool local) 
{
	Sprite* s2_spr = static_cast<Sprite*>(spr);
	if (s2_spr->GetSymbol()->Type() == SYM_PROXY) {
		const ProxySymbol* proxy_sym = VI_DOWNCASTING<const ProxySymbol*>(s2_spr->GetSymbol());
		const std::vector<std::pair<const Actor*, Sprite*> >& items = proxy_sym->GetItems();
		for (int i = 0, n = items.size(); i < n; ++i) {
			_p3d_set_local(items[i].second, local);
		}
	} else {
		_p3d_set_local(s2_spr, local);
	}
}

static bool
_p3d_set_loop(Sprite* spr, bool loop)
{
	if (spr->GetSymbol()->Type() != SYM_PARTICLE3D) {
		return false;
	}

	Particle3dSprite* p3d_spr = static_cast<Particle3dSprite*>(spr);
	p3d_spr->SetLoop(loop);

	return true;
}

extern "C"
void s2_spr_p3d_set_loop(void* spr, bool loop)
{
	Sprite* s2_spr = static_cast<Sprite*>(spr);
	if (s2_spr->GetSymbol()->Type() == SYM_PROXY) {
		const ProxySymbol* proxy_sym = VI_DOWNCASTING<const ProxySymbol*>(s2_spr->GetSymbol());
		const std::vector<std::pair<const Actor*, Sprite*> >& items = proxy_sym->GetItems();
		for (int i = 0, n = items.size(); i < n; ++i) {
			_p3d_set_loop(items[i].second, loop);
		}
	} else {
		_p3d_set_loop(s2_spr, loop);
	}
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

static bool
_p3d_update(Sprite* spr, float dt)
{
	if (spr->GetSymbol()->Type() != SYM_PARTICLE3D) {
		return false;
	}

	Particle3dSprite* p3d_spr = static_cast<Particle3dSprite*>(spr);
	if (const Particle3dEmitter* et = p3d_spr->GetEmitter()) {
		const_cast<Particle3dEmitter*>(et)->Update(et->GetTime() + dt);
	}

	return true;
}

extern "C"
void s2_spr_p3d_update(void* spr, float dt)
{
	Sprite* s2_spr = static_cast<Sprite*>(spr);
	if (s2_spr->GetSymbol()->Type() == SYM_PROXY) {
		const ProxySymbol* proxy_sym = VI_DOWNCASTING<const ProxySymbol*>(s2_spr->GetSymbol());
		const std::vector<std::pair<const Actor*, Sprite*> >& items = proxy_sym->GetItems();
		for (int i = 0, n = items.size(); i < n; ++i) {
			s2_spr_p3d_update(items[i].second, dt);
		}
	} else {
		s2_spr_p3d_update(s2_spr, dt);
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
	Sprite* s2_spr = static_cast<Sprite*>(spr);
	if (s2_spr->GetSymbol()->Type() == SYM_PROXY) {
		const ProxySymbol* proxy_sym = VI_DOWNCASTING<const ProxySymbol*>(s2_spr->GetSymbol());
		const std::vector<std::pair<const Actor*, Sprite*> >& items = proxy_sym->GetItems();
		for (int i = 0, n = items.size(); i < n; ++i) {
			static_cast<Sprite*>(items[i].second)->SetDTexDisable(!enable);
		}
	} else {
		static_cast<Sprite*>(spr)->SetDTexDisable(!enable);
	}
}

static void
_set_dtex_force_cached(Sprite* spr, bool cache)
{
	spr->SetDTexForceCached(cache);
	if (cache) {
		spr->SetDTexForceCachedDirty(true);
	}
}

extern "C"
void s2_spr_set_dtex_force_cached(void* spr, bool cache)
{
	Sprite* s2_spr = static_cast<Sprite*>(spr);
	if (s2_spr->GetSymbol()->Type() == SYM_PROXY) {
		const ProxySymbol* proxy_sym = VI_DOWNCASTING<const ProxySymbol*>(s2_spr->GetSymbol());
		const std::vector<std::pair<const Actor*, Sprite*> >& items = proxy_sym->GetItems();
		for (int i = 0, n = items.size(); i < n; ++i) {
			_set_dtex_force_cached(items[i].second, cache);
		}
	} else {
		_set_dtex_force_cached(s2_spr, cache);
	}
}

extern "C"
void s2_spr_set_dtex_force_cached_dirty(void* spr, bool dirty)
{
	Sprite* s2_spr = static_cast<Sprite*>(spr);
	if (s2_spr->GetSymbol()->Type() == SYM_PROXY) {
		const ProxySymbol* proxy_sym = VI_DOWNCASTING<const ProxySymbol*>(s2_spr->GetSymbol());
		const std::vector<std::pair<const Actor*, Sprite*> >& items = proxy_sym->GetItems();
		for (int i = 0, n = items.size(); i < n; ++i) {
			static_cast<Sprite*>(items[i].second)->SetDTexForceCachedDirty(dirty);
		}
	} else {
		static_cast<Sprite*>(spr)->SetDTexForceCachedDirty(dirty);
	}	
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
	const Sprite* s2_spr = s2_actor->GetSpr();
	UpdateParams up(s2_actor);
	up.SetPrevMat(get_actor_world_mat(s2_actor->GetParent()));
	const_cast<Sprite*>(s2_spr)->Update(up);
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
	const Actor* s2_actor = static_cast<const Actor*>(actor);
	const Sprite* s2_spr = s2_actor->GetSpr();
	SetStaticFrameVisitor visitor(frame);
	SprVisitorParams vp;
	vp.actor = s2_actor;
	bool old_inherit_update = s2_spr->IsInheritUpdate();
	s2_spr->SetInheritUpdate(true);
	s2_spr->Traverse(visitor, vp);
	s2_spr->SetInheritUpdate(old_inherit_update);
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

static int
_actor_mount(const Actor* parent, Sprite* old_child, const Actor* new_child) {
	if (!old_child) {
		return -1;		
	}
	if (old_child->GetSymbol()->Type() != SYM_ANCHOR) {
		return -2;
	}
	AnchorSprite* anchor_spr = VI_DOWNCASTING<AnchorSprite*>(old_child);
	anchor_spr->AddAnchor(new_child, parent);
	return 0;
}

// ret: 0 ok, -1 no child with name, -2 child isn't anchor
extern "C"
int s2_actor_mount(const void* parent, const char* name, const void* child) {
	const Actor* p_actor = static_cast<const Actor*>(parent);
	const Sprite* p_spr = p_actor->GetSpr();
	const Actor* new_actor = static_cast<const Actor*>(child);
	Sprite* old_spr = p_spr->FetchChild(name, p_actor);
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
			int _ret = _actor_mount(p_actor, items[i].second, new_actor);
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
bool s2_actor_get_force_update(void* actor) {
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
	return s2_spr->IsForceUpdate();
}

extern "C"
void s2_actor_set_force_update(void* actor, bool force) {
	const Actor* s2_actor = static_cast<const Actor*>(actor);
	const Sprite* s2_spr = s2_actor->GetSpr();
	if (s2_spr->GetSymbol()->Type() == SYM_ANCHOR) {
		const AnchorSprite* anchor_spr = VI_DOWNCASTING<const AnchorSprite*>(s2_spr);
		const Actor* real = anchor_spr->QueryAnchor(s2_actor);
		if (real) {
			s2_spr = real->GetSpr();
		}
	}
	if (s2_spr->GetSymbol()->Type() == SYM_PROXY) {
		const ProxySymbol* proxy_sym = VI_DOWNCASTING<const ProxySymbol*>(s2_spr->GetSymbol());
		const std::vector<std::pair<const Actor*, Sprite*> >& items = proxy_sym->GetItems();
		for (int i = 0, n = items.size(); i < n; ++i) {
			items[i].second->SetForceUpdate(force);
		}
	} else {
		s2_spr->SetForceUpdate(force);
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

static sm::rect
_get_actor_aabb(const Actor* actor) {
	const sm::rect& src = actor->GetAABB().GetRect();

	sm::vec2 min(src.xmin, src.ymin),
		     max(src.xmax, src.ymax);
	S2_MAT mat = actor->GetLocalMat() * actor->GetSpr()->GetLocalMat();
	min = mat * min;
	max = mat * max;

	sm::rect ret;
	ret.Combine(min);
	ret.Combine(max);
	return ret;
}

extern "C"
void s2_actor_get_aabb(const void* actor, float aabb[4]) {
	const Actor* s2_actor = static_cast<const Actor*>(actor);
	const Sprite* s2_spr = s2_actor->GetSpr();
	sm::rect rect;
	if (s2_spr->GetSymbol()->Type() == SYM_PROXY) {
		const ProxySymbol* proxy_sym = VI_DOWNCASTING<const ProxySymbol*>(s2_spr->GetSymbol());
		const std::vector<std::pair<const Actor*, Sprite*> >& items = proxy_sym->GetItems();
		for (int i = 0, n = items.size(); i < n; ++i) {
			const Actor* child_actor = items[i].second->QueryActor(items[i].first);
			rect.Combine(_get_actor_aabb(child_actor));
		}
	} else {
		rect = _get_actor_aabb(s2_actor);
	}
	aabb[0] = rect.xmin;
	aabb[1] = rect.ymin;
	aabb[2] = rect.xmax;
	aabb[3] = rect.ymax;
}

extern "C"
void s2_actor_set_pos(void* actor, float x, float y) {
	Actor* s2_actor = static_cast<Actor*>(actor);
	if (s2_actor->GetSpr()->GetSymbol()->Type() == SYM_PROXY) {
		const ProxySymbol* proxy_sym = VI_DOWNCASTING<const ProxySymbol*>(s2_actor->GetSpr()->GetSymbol());
		const std::vector<std::pair<const Actor*, Sprite*> >& items = proxy_sym->GetItems();
		for (int i = 0, n = items.size(); i < n; ++i) {
			const Actor* child_actor = items[i].second->QueryActor(items[i].first);
			const_cast<Actor*>(child_actor)->SetPosition(sm::vec2(x, y));
		}
	} else {
		s2_actor->SetPosition(sm::vec2(x, y));
	}
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
	if (s2_actor->GetSpr()->GetSymbol()->Type() == SYM_PROXY) {
		const ProxySymbol* proxy_sym = VI_DOWNCASTING<const ProxySymbol*>(s2_actor->GetSpr()->GetSymbol());
		const std::vector<std::pair<const Actor*, Sprite*> >& items = proxy_sym->GetItems();
		for (int i = 0, n = items.size(); i < n; ++i) {
			const Actor* child_actor = items[i].second->QueryActor(items[i].first);
			const_cast<Actor*>(child_actor)->SetAngle(angle);
		}
	} else {
		s2_actor->SetAngle(angle);
	}
}

extern "C"
float s2_actor_get_angle(void* actor) {
	Actor* s2_actor = static_cast<Actor*>(actor);
	return s2_actor->GetAngle();
}

extern "C"
void s2_actor_set_scale(void* actor, float sx, float sy) {
	Actor* s2_actor = static_cast<Actor*>(actor);
	if (s2_actor->GetSpr()->GetSymbol()->Type() == SYM_PROXY) {
		const ProxySymbol* proxy_sym = VI_DOWNCASTING<const ProxySymbol*>(s2_actor->GetSpr()->GetSymbol());
		const std::vector<std::pair<const Actor*, Sprite*> >& items = proxy_sym->GetItems();
		for (int i = 0, n = items.size(); i < n; ++i) {
			const Actor* child_actor = items[i].second->QueryActor(items[i].first);
			const_cast<Actor*>(child_actor)->SetScale(sm::vec2(sx, sy));
		}
	} else {
		s2_actor->SetScale(sm::vec2(sx, sy));
	}
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

static void _set_visible(const Actor* actor, bool visible) {
	if (visible != actor->IsVisible()) {
		actor->SetFlattenDirtyToRoot();
		actor->SetVisible(visible);
	}
}

extern "C"
void s2_actor_set_visible(void* actor, bool visible) {
	Actor* s2_actor = static_cast<Actor*>(actor);
	if (s2_actor->GetSpr()->GetSymbol()->Type() == SYM_PROXY) {
		const ProxySymbol* proxy_sym = VI_DOWNCASTING<const ProxySymbol*>(s2_actor->GetSpr()->GetSymbol());
		const std::vector<std::pair<const Actor*, Sprite*> >& items = proxy_sym->GetItems();
		for (int i = 0, n = items.size(); i < n; ++i) {
			const Actor* child_actor = items[i].second->QueryActor(items[i].first);
			_set_visible(child_actor, visible);
		}
	} else {
		_set_visible(s2_actor, visible);
	}
}

extern "C"
bool s2_actor_get_editable(void* actor) {
	Actor* s2_actor = static_cast<Actor*>(actor);
	return s2_actor->IsEditable();
}

extern "C"
void s2_actor_set_editable(void* actor, bool editable) {
	Actor* s2_actor = static_cast<Actor*>(actor);
	if (s2_actor->GetSpr()->GetSymbol()->Type() == SYM_PROXY) {
		const ProxySymbol* proxy_sym = VI_DOWNCASTING<const ProxySymbol*>(s2_actor->GetSpr()->GetSymbol());
		const std::vector<std::pair<const Actor*, Sprite*> >& items = proxy_sym->GetItems();
		for (int i = 0, n = items.size(); i < n; ++i) {
			const Actor* child_actor = items[i].second->QueryActor(items[i].first);
			child_actor->SetEditable(editable);
		}
	} else {
		s2_actor->SetEditable(editable);
	}	
}

extern "C"
uint32_t s2_actor_get_col_mul(void* actor) {
	Actor* s2_actor = static_cast<Actor*>(actor);
	return s2_actor->GetColor().GetMulABGR();
}

static void
_set_col_mul(const Actor* actor, uint32_t abgr) {
	if (actor->GetColor().GetMulABGR() != abgr) 
	{
		RenderColor rc = actor->GetColor();
		rc.SetMulABGR(abgr);
		const_cast<Actor*>(actor)->SetColor(rc);
	}
}

extern "C"
void s2_actor_set_col_mul(void* actor, uint32_t abgr) {
	Actor* s2_actor = static_cast<Actor*>(actor);
	if (s2_actor->GetSpr()->GetSymbol()->Type() == SYM_PROXY) {
		const ProxySymbol* proxy_sym = VI_DOWNCASTING<const ProxySymbol*>(s2_actor->GetSpr()->GetSymbol());
		const std::vector<std::pair<const Actor*, Sprite*> >& items = proxy_sym->GetItems();
		for (int i = 0, n = items.size(); i < n; ++i) {
			const Actor* child_actor = items[i].second->QueryActor(items[i].first);
			_set_col_mul(child_actor, abgr);
		}
	} else {
		_set_col_mul(s2_actor, abgr);
	}
}

extern "C"
uint32_t s2_actor_get_col_add(void* actor) {
	Actor* s2_actor = static_cast<Actor*>(actor);
	return s2_actor->GetColor().GetAddABGR();
}

static void
_set_col_add(const Actor* actor, uint32_t abgr) {
	if (actor->GetColor().GetAddABGR() != abgr) 
	{
		RenderColor rc = actor->GetColor();
		rc.SetAddABGR(abgr);
		const_cast<Actor*>(actor)->SetColor(rc);
	}
}

extern "C"
void s2_actor_set_col_add(void* actor, uint32_t abgr) {
	Actor* s2_actor = static_cast<Actor*>(actor);
	if (s2_actor->GetSpr()->GetSymbol()->Type() == SYM_PROXY) {
		const ProxySymbol* proxy_sym = VI_DOWNCASTING<const ProxySymbol*>(s2_actor->GetSpr()->GetSymbol());
		const std::vector<std::pair<const Actor*, Sprite*> >& items = proxy_sym->GetItems();
		for (int i = 0, n = items.size(); i < n; ++i) {
			const Actor* child_actor = items[i].second->QueryActor(items[i].first);
			_set_col_add(child_actor, abgr);
		}
	} else {
		_set_col_add(s2_actor, abgr);
	}
}

extern "C"
void s2_actor_get_col_map(void* actor, uint32_t* rmap, uint32_t* gmap, uint32_t* bmap) {
	Actor* s2_actor = static_cast<Actor*>(actor);
	*rmap = s2_actor->GetColor().GetRMapABGR();
	*gmap = s2_actor->GetColor().GetGMapABGR();
	*bmap = s2_actor->GetColor().GetBMapABGR();
}

static void
_set_col_map(const Actor* actor, uint32_t rmap, uint32_t gmap, uint32_t bmap) {
	if (actor->GetColor().GetRMapABGR() != rmap ||
		actor->GetColor().GetGMapABGR() != gmap ||
		actor->GetColor().GetBMapABGR() != bmap) 
	{
		RenderColor rc = actor->GetColor();
		rc.SetRMapABGR(rmap);
		rc.SetGMapABGR(gmap);
		rc.SetBMapABGR(bmap);
		const_cast<Actor*>(actor)->SetColor(rc);
	}
}

extern "C"
void s2_actor_set_col_map(void* actor, uint32_t rmap, uint32_t gmap, uint32_t bmap) {
	Actor* s2_actor = static_cast<Actor*>(actor);
	if (s2_actor->GetSpr()->GetSymbol()->Type() == SYM_PROXY) {
		const ProxySymbol* proxy_sym = VI_DOWNCASTING<const ProxySymbol*>(s2_actor->GetSpr()->GetSymbol());
		const std::vector<std::pair<const Actor*, Sprite*> >& items = proxy_sym->GetItems();
		for (int i = 0, n = items.size(); i < n; ++i) {
			const Actor* child_actor = items[i].second->QueryActor(items[i].first);
			_set_col_map(child_actor, rmap, gmap, bmap);
		}
	} else {
		_set_col_map(s2_actor, rmap, gmap, bmap);
	}
}

static void 
_set_filter(const Actor* actor, int mode) {
	const RenderFilter* filter = actor->GetShader().GetFilter();
	FilterMode ori = FM_NULL;
	if (filter) {
		ori = filter->GetMode();
	}
	if (ori == mode) {
		return;
	}

	RenderShader shader = actor->GetShader();
	shader.SetFilter(FilterMode(mode));
	const_cast<Actor*>(actor)->SetShader(shader);	
}

extern "C"
void s2_actor_set_filter(void* actor, int mode) {
	Actor* s2_actor = static_cast<Actor*>(actor);
	if (s2_actor->GetSpr()->GetSymbol()->Type() == SYM_PROXY) {
		const ProxySymbol* proxy_sym = VI_DOWNCASTING<const ProxySymbol*>(s2_actor->GetSpr()->GetSymbol());
		const std::vector<std::pair<const Actor*, Sprite*> >& items = proxy_sym->GetItems();
		for (int i = 0, n = items.size(); i < n; ++i) {
			const Actor* child_actor = items[i].second->QueryActor(items[i].first);
			_set_filter(child_actor, mode);
		}
	} else {
		_set_filter(s2_actor, mode);
	}
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

static void
_set_text(const Actor* actor, const char* text) {
	if (actor->GetSpr()->GetSymbol()->Type() == SYM_TEXTBOX) {
		const TextboxActor* textbox = static_cast<const TextboxActor*>(actor);
		const_cast<TextboxActor*>(textbox)->SetText(text);	
	}
}

extern "C"
void s2_actor_set_text(void* actor, const char* text) {	
	Actor* s2_actor = static_cast<Actor*>(actor);
	if (s2_actor->GetSpr()->GetSymbol()->Type() == SYM_PROXY) {
		const ProxySymbol* proxy_sym = VI_DOWNCASTING<const ProxySymbol*>(s2_actor->GetSpr()->GetSymbol());
		const std::vector<std::pair<const Actor*, Sprite*> >& items = proxy_sym->GetItems();
		for (int i = 0, n = items.size(); i < n; ++i) {
			const Actor* child_actor = items[i].second->QueryActor(items[i].first);
			_set_text(child_actor, text);
		}
	} else {
		_set_text(s2_actor, text);
	}
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

static void
_scale9_resize(const Actor* actor, int w, int h) {
	if (actor->GetSpr()->GetSymbol()->Type() == SYM_SCALE9) {
		const Scale9Actor* s9_actor = static_cast<const Scale9Actor*>(actor);
		const_cast<Scale9Actor*>(s9_actor)->Resize(w, h);	
	}
}

extern "C"
void s2_actor_scale9_resize(void* actor, int w, int h) {
	Actor* s2_actor = static_cast<Actor*>(actor);
	if (s2_actor->GetSpr()->GetSymbol()->Type() == SYM_PROXY) {
		const ProxySymbol* proxy_sym = VI_DOWNCASTING<const ProxySymbol*>(s2_actor->GetSpr()->GetSymbol());
		const std::vector<std::pair<const Actor*, Sprite*> >& items = proxy_sym->GetItems();
		for (int i = 0, n = items.size(); i < n; ++i) {
			const Actor* child_actor = items[i].second->QueryActor(items[i].first);
			_scale9_resize(child_actor, w, h);
		}
	} else {
		_scale9_resize(s2_actor, w, h);
	}
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

}
