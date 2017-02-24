#include "c_wrap_s2.h"

#include "RenderCtxStack.h"
#include "S2_Actor.h"
#include "ActorFactory.h"
#include "S2_Sprite.h"
#include "S2_Symbol.h"
#include "SymType.h"
#include "TextboxActor.h"
#include "ActorLUT.h"
#include "PointQuery2Visitor.h"
#include "RenderParams.h"
#include "DrawNode.h"
#include "RenderFilter.h"
#include "BoundingBox.h"
#include "S2_RVG.h"
#include "PointQueryVisitor.h"
#include "SprTimer.h"
#include "SprVisitorParams.h"
#include "RenderTargetMgr.h"
#include "RenderTarget.h"
#include "RenderScissor.h"
#include "Blackboard.h"

#include "ComplexSymbol.h"
#include "ComplexSprite.h"
#include "AnimSymbol.h"
#include "AnimSprite.h"
#include "TextboxSprite.h"
#include "Scale9Sprite.h"

#include <gtxt_label.h>
#include <shaderlab/ShaderMgr.h>
#include <shaderlab/Sprite2Shader.h>
#include <shaderlab/FilterShader.h>

#include <iostream>

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

/************************************************************************/
/* sprite                                                               */
/************************************************************************/

extern "C"
void s2_spr_draw(const void* spr, float x, float y, float angle, float sx, float sy)
{
	RenderParams params;
	params.mt.SetTransformation(x, y, angle, sx, sy, 0, 0, 0, 0);

	const RenderContext* ctx = RenderCtxStack::Instance()->Top();
	float hw = ctx->GetScreenWidth() * 0.5f,
		  hh = ctx->GetScreenHeight() * 0.5f;
	params.view_region.xmin = -hw;
	params.view_region.ymin = -hh;
	params.view_region.xmax =  hw;
	params.view_region.ymax =  hh;

	const Sprite* s2_spr = static_cast<const Sprite*>(spr);
	DrawNode::Draw(s2_spr, params);
}

extern "C"
void s2_spr_update(void* spr) {
	static_cast<Sprite*>(spr)->Update(RenderParams());
}

extern "C"
void* s2_spr_fetch_child(const void* spr, const char* name) {
	const Sprite* child = static_cast<const Sprite*>(spr)->FetchChild(name);
	if (child) {
		return const_cast<Sprite*>(child);
	} else {
		return NULL;
	}
}

extern "C"
void* s2_spr_fetch_child_by_index(const void* spr, int idx) {
	const Sprite* child = static_cast<const Sprite*>(spr)->FetchChild(idx);
	if (child) {
		return const_cast<Sprite*>(child);
	} else {
		return NULL;
	}
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

extern "C"
bool s2_spr_get_editable(void* spr) {
	return static_cast<Sprite*>(spr)->IsEditable();	
}

extern "C"
void s2_spr_set_editable(void* spr, bool editable) {
	static_cast<Sprite*>(spr)->SetEditable(editable);
}

extern "C"
bool s2_spr_get_force_up_frame(void* spr) {
	return static_cast<Sprite*>(spr)->IsForceUpFrame();
}

extern "C"
void s2_spr_set_force_up_frame(void* spr, bool force) {
	static_cast<Sprite*>(spr)->SetForceUpFrame(force);
}

extern "C"
void s2_spr_set_frame(void* spr, int frame) {
	Sprite* s2_spr = static_cast<Sprite*>(spr);
	s2_spr->SetFrame(frame);
}

extern "C"
void s2_spr_set_action(void* spr, const char* action) {
	Sprite* s2_spr = static_cast<Sprite*>(spr);
	if (s2_spr->GetSymbol()->Type() == SYM_COMPLEX) {
		ComplexSprite* complex = VI_DOWNCASTING<ComplexSprite*>(s2_spr);
		complex->SetAction(action);
	}
}

extern "C"
int s2_spr_get_frame(void* spr) {
	Sprite* s2_spr = static_cast<Sprite*>(spr);
	if (s2_spr->GetSymbol()->Type() == SYM_ANIMATION) {
		AnimSprite* anim = VI_DOWNCASTING<AnimSprite*>(s2_spr);
		return anim->GetFrame();
	} else {
		return -1;
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
		return -1;
	}
}

extern "C"
int s2_spr_get_component_count(void* spr) {
	Sprite* s2_spr = static_cast<Sprite*>(spr);
	if (s2_spr->GetSymbol()->Type() == SYM_COMPLEX) {
		ComplexSprite* complex = VI_DOWNCASTING<ComplexSprite*>(s2_spr);
		const ComplexSymbol* sym = VI_DOWNCASTING<const ComplexSymbol*>(complex->GetSymbol());
		return sym->GetActionChildren(complex->GetAction()).size();
	} else {
		return -1;
	}
}

extern "C"
void s2_spr_set_filter(void* spr, int mode) {
	Sprite* s2_spr = static_cast<Sprite*>(spr);
	const RenderFilter* filter = s2_spr->GetShader().GetFilter();
	FilterMode ori = FM_NULL;
	if (filter) {
		ori = filter->GetMode();
	}
	if (ori == mode) {
		return;
	}

	RenderShader shader = s2_spr->GetShader();
	shader.SetFilter(FilterMode(mode));
	s2_spr->SetShader(shader);
}

extern "C"
const char* s2_spr_get_text(void* spr) {
	Sprite* s2_spr = static_cast<Sprite*>(spr);
	if (s2_spr->GetSymbol()->Type() == SYM_TEXTBOX) {
		TextboxSprite* text = VI_DOWNCASTING<TextboxSprite*>(s2_spr);
		return text->GetText().c_str();
	} else {
		return NULL;
	}
}

extern "C"
void s2_spr_set_text(void* spr, const char* text) {
	Sprite* s2_spr = static_cast<Sprite*>(spr);
	if (s2_spr->GetSymbol()->Type() == SYM_TEXTBOX) {
		TextboxSprite* text_spr = VI_DOWNCASTING<TextboxSprite*>(s2_spr);
		text_spr->SetText(text);
	}
}

extern "C"
void s2_spr_get_aabb(const void* spr, float aabb[4]) {
	const Sprite* s2_spr = static_cast<const Sprite*>(spr);
	sm::rect sz = s2_spr->GetBounding()->GetSize();
	aabb[0] = sz.xmin;
	aabb[1] = sz.ymin;
	aabb[2] = sz.xmax;
	aabb[3] = sz.ymax;
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

	s2_spr->Traverse(visitor, SprVisitorParams());
	const Sprite* ret = visitor.GetSelectedSpr();
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

	return const_cast<Sprite*>(ret);
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

static void
_init_gtxt_label_style(struct gtxt_label_style* dst, const Textbox& src) {
	dst->width  = src.width;
	dst->height = src.height;

	dst->align_h = src.align_hori;
	dst->align_v = src.align_vert;

	dst->space_h = src.space_hori;
	dst->space_v = src.space_vert;

	dst->gs.font = src.font_type;
	dst->gs.font_size = src.font_size;
	dst->gs.font_color.integer = src.font_color.ToRGBA();

	dst->gs.edge = src.has_edge;
	dst->gs.edge_size = src.edge_size;
	dst->gs.edge_color.integer = src.edge_color.ToRGBA();

	dst->overflow = src.overflow;
}

extern "C"
bool s2_spr_get_text_size(const void* spr, float* w, float* h) {
	const Sprite* s2_spr = static_cast<const Sprite*>(spr);
	if (s2_spr->GetSymbol()->Type() != SYM_TEXTBOX) {
		return false;
	}

	const TextboxSprite* text_spr = static_cast<const TextboxSprite*>(s2_spr);

	struct gtxt_label_style style;
	_init_gtxt_label_style(&style, text_spr->GetTextbox());
	gtxt_get_label_size(text_spr->GetText().c_str(), &style, w, h);	

	return true;
}

extern "C"
bool s2_spr_get_scissor(const void* spr, float* x, float* y, float* w, float* h)
{
	const Sprite* s2_spr = static_cast<const Sprite*>(spr);
	if (s2_spr->GetSymbol()->Type() != SYM_COMPLEX) {
		return false;
	}

	const ComplexSprite* complex_spr = static_cast<const ComplexSprite*>(s2_spr);
	const ComplexSymbol* sym = static_cast<const ComplexSymbol*>(complex_spr->GetSymbol());
	const sm::rect& scissor = sym->GetScissor();
	*x = scissor.xmin;
	*y = scissor.ymin;
	*w = scissor.xmax - scissor.xmin;
	*h = scissor.ymax - scissor.ymin;

	return true;
}

extern "C"
bool s2_spr_set_scissor(void* spr, float x, float y, float w, float h)
{
	const Sprite* s2_spr = static_cast<const Sprite*>(spr);
	if (s2_spr->GetSymbol()->Type() != SYM_COMPLEX) {
		return false;
	}

	const ComplexSprite* complex_spr = static_cast<const ComplexSprite*>(s2_spr);
	const ComplexSymbol* sym = static_cast<const ComplexSymbol*>(complex_spr->GetSymbol());
	sm::rect scissor;
	scissor.xmin = x;
	scissor.ymin = y;
	scissor.xmax = x + w;
	scissor.ymax = y + h;

	return true;
}

extern "C"
void s2_spr_scale9_resize(void* spr, int w, int h)
{
	Sprite* s2_spr = static_cast<Sprite*>(spr);
	if (s2_spr->GetSymbol()->Type() != SYM_SCALE9) {
		return;
	}

	Scale9Sprite* s9_spr = static_cast<Scale9Sprite*>(s2_spr);
	s9_spr->Resize(w, h);
}

/************************************************************************/
/* actor                                                                */
/************************************************************************/

extern "C"
void* s2_point_query_actor(const void* parent_actor, float x, float y, float mat[6]) {
	const Actor* parent = static_cast<const Actor*>(parent_actor);

	PointQuery2Visitor visitor(sm::vec2(x, y));
	parent->GetSpr()->Traverse(visitor, SprVisitorParams());

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

	return visitor.GetSelectedActor();
}

extern "C"
void* s2_get_actor(const void* parent_actor, void* child_spr) {
	const Actor* parent = static_cast<const Actor*>(parent_actor);
	Sprite* child = static_cast<Sprite*>(child_spr);
	if (parent) {
		return ActorFactory::Instance()->Create(parent->GetTreePath(), child);
	} else {
		SprTreePath path;
		return ActorFactory::Instance()->Create(path, child);
	}
}

extern "C"
int s2_get_actor_count() {
	return ActorLUT::Instance()->Count();
}

extern "C"
void s2_actor_set_proxy(void* actor, void* spr) {
	static_cast<Actor*>(actor)->SetProxy(static_cast<Sprite*>(spr));
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

extern "C"
void s2_actor_set_pos(void* actor, float x, float y) {
	Actor* s2_actor = static_cast<Actor*>(actor);
	s2_actor->SetPosition(sm::vec2(x, y));
}

extern "C"
void s2_actor_set_angle(void* actor, float angle) {
	Actor* s2_actor = static_cast<Actor*>(actor);
	s2_actor->SetAngle(angle);
}

extern "C"
void s2_actor_set_scale(void* actor, float sx, float sy) {
	Actor* s2_actor = static_cast<Actor*>(actor);
	s2_actor->SetScale(sm::vec2(sx, sy));
}

extern "C"
void* s2_actor_get_parent(void* actor) {
	Actor* s2_actor = static_cast<Actor*>(actor);
	if (s2_actor->GetTreePath().Empty()) {
		return NULL;
	}

	SprTreePath path = s2_actor->GetTreePath();
	path.Pop();
	return ActorLUT::Instance()->Query(path);
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
uint32_t s2_actor_get_col_mul(void* actor) {
	Actor* s2_actor = static_cast<Actor*>(actor);
	return s2_actor->GetColor().GetMul().ToRGBA();
}

extern "C"
void s2_actor_set_col_mul(void* actor, uint32_t rgba) {
	Actor* s2_actor = static_cast<Actor*>(actor);
	if (s2_actor->GetColor().GetMul().ToRGBA() == rgba) {
		return;
	}

	RenderColor col = s2_actor->GetColor();
	col.SetMul(Color(rgba));
	s2_actor->SetColor(col);
}

extern "C"
uint32_t s2_actor_get_col_add(void* actor) {
	Actor* s2_actor = static_cast<Actor*>(actor);
	return s2_actor->GetColor().GetAdd().ToRGBA();
}

extern "C"
void s2_actor_set_col_add(void* actor, uint32_t rgba) {
	Actor* s2_actor = static_cast<Actor*>(actor);
	if (s2_actor->GetColor().GetAdd().ToRGBA() == rgba) {
		return;
	}

	RenderColor col = s2_actor->GetColor();
	col.SetAdd(Color(rgba));
	s2_actor->SetColor(col);
}

extern "C"
void s2_actor_get_col_map(void* actor, uint32_t* rmap, uint32_t* gmap, uint32_t* bmap) {
	Actor* s2_actor = static_cast<Actor*>(actor);
	*rmap = s2_actor->GetColor().GetMapR().ToRGBA();
	*gmap = s2_actor->GetColor().GetMapG().ToRGBA();
	*bmap = s2_actor->GetColor().GetMapB().ToRGBA();
}

extern "C"
void s2_actor_set_col_map(void* actor, uint32_t rmap, uint32_t gmap, uint32_t bmap) {
	Actor* s2_actor = static_cast<Actor*>(actor);
	if (s2_actor->GetColor().GetMapR().ToRGBA() == rmap &&
		s2_actor->GetColor().GetMapG().ToRGBA() == gmap &&
		s2_actor->GetColor().GetMapB().ToRGBA() == bmap) {
		return;
	}

	RenderColor col = s2_actor->GetColor();
	col.SetMapR(Color(rmap));
	col.SetMapG(Color(gmap));
	col.SetMapB(Color(bmap));
	s2_actor->SetColor(col);
}

/************************************************************************/
/* others                                                               */
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

static void _draw(const struct rect_tex* dst, const struct rect_tex* src, int src_tex_id)
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
void s2_rt_draw_from(void* rt, const struct rect_tex* dst, const struct rect_tex* src, int src_tex_id)
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
void s2_rt_draw_to(void* rt, const struct rect_tex* dst, const struct rect_tex* src)
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

}