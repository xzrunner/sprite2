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

#include "ComplexSymbol.h"
#include "ComplexSprite.h"
#include "AnimSymbol.h"
#include "AnimSprite.h"
#include "TextboxSprite.h"

#include <gtxt_label.h>

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
int s2_get_spr_count() {
	return s2::Sprite::GetCount();
}

extern "C"
void s2_spr_draw(const void* spr, float x, float y, float angle, float sx, float sy)
{
	RenderParams params;
	params.mt.SetTransformation(x, y, angle, sx, sy, 0, 0, 0, 0);
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
bool s2_spr_get_visible(void* spr) {
	return static_cast<Sprite*>(spr)->IsVisible();
}

extern "C"
void s2_spr_set_visible(void* spr, bool visible) {
	static_cast<Sprite*>(spr)->SetVisible(visible);
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
void s2_spr_set_frame(void* spr, int frame) {
	Sprite* s2_spr = static_cast<Sprite*>(spr);
	if (s2_spr->GetSymbol()->Type() == SYM_ANIMATION) {
		AnimSprite* anim = VI_DOWNCASTING<AnimSprite*>(s2_spr);
		anim->SetFrame(frame);
	}
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
		return sym->GetChildren().size();
	} else {
		return -1;
	}
}

extern "C"
uint32_t s2_spr_get_col_mul(void* spr) {
	Sprite* s2_spr = static_cast<Sprite*>(spr);
	return s2_spr->GetColor().mul.ToRGBA();
}

extern "C"
uint32_t s2_spr_get_col_add(void* spr) {
	Sprite* s2_spr = static_cast<Sprite*>(spr);
	return s2_spr->GetColor().add.ToRGBA();
}

extern "C"
void s2_spr_get_col_map(void* spr, uint32_t* rmap, uint32_t* gmap, uint32_t* bmap) {
	Sprite* s2_spr = static_cast<Sprite*>(spr);
	*rmap = s2_spr->GetColor().rmap.ToRGBA();
	*gmap = s2_spr->GetColor().gmap.ToRGBA();
	*bmap = s2_spr->GetColor().bmap.ToRGBA();
}

extern "C"
void s2_spr_set_col_mul(void* spr, uint32_t rgba) {
	Sprite* s2_spr = static_cast<Sprite*>(spr);
	if (s2_spr->GetColor().mul.ToRGBA() == rgba) {
		return;
	}

	RenderColor col = s2_spr->GetColor();
	col.mul.FromRGBA(rgba);
	s2_spr->SetColor(col);
}

extern "C"
void s2_spr_set_col_add(void* spr, uint32_t rgba) {
	Sprite* s2_spr = static_cast<Sprite*>(spr);
	if (s2_spr->GetColor().add.ToRGBA() == rgba) {
		return;
	}

	RenderColor col = s2_spr->GetColor();
	col.add.FromRGBA(rgba);
	s2_spr->SetColor(col);
}

extern "C"
void s2_spr_set_col_map(void* spr, uint32_t rmap, uint32_t gmap, uint32_t bmap) {
	Sprite* s2_spr = static_cast<Sprite*>(spr);
	if (s2_spr->GetColor().rmap.ToRGBA() == rmap &&
		s2_spr->GetColor().gmap.ToRGBA() == gmap &&
		s2_spr->GetColor().bmap.ToRGBA() == bmap) {
			return;
	}

	RenderColor col = s2_spr->GetColor();
	col.rmap.FromRGBA(rmap);
	col.gmap.FromRGBA(gmap);
	col.bmap.FromRGBA(bmap);
	s2_spr->SetColor(col);
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
	sm::mat4 outer;
	outer.x[0] = mat[0];
	outer.x[1] = mat[1];
	outer.x[4] = mat[2];
	outer.x[5] = mat[3];
	outer.x[12]= mat[4];
	outer.x[13]= mat[5];

	sm::mat4 m;
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
	sm::mat4 _mat;
	s2_spr->Traverse(visitor, &_mat);
	const Sprite* ret = visitor.GetSelectedSpr();
	if (!ret) {
		return NULL;
	}

	_mat = visitor.GetSelectedMat();
	mat[0] = _mat.x[0];
	mat[1] = _mat.x[1];
	mat[2] = _mat.x[4];
	mat[3] = _mat.x[5];
	mat[4] = _mat.x[12];
	mat[5] = _mat.x[13];

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

/************************************************************************/
/* actor                                                                */
/************************************************************************/

extern "C"
void* s2_point_query_actor(const void* parent_actor, float x, float y, float mat[6]) {
	const Actor* parent = static_cast<const Actor*>(parent_actor);
	PointQuery2Visitor visitor(parent->GetTreePath(), sm::vec2(x, y));
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
		SprTreePath path;
		return ActorFactory::Instance()->Create(path, child);
	}
}

extern "C"
int s2_get_actor_count() {
	return ActorLUT::Instance()->Count();
}

extern "C"
void s2_actor_mount_child(void* parent, const char* name, void* child) {
	Actor* actor = static_cast<Actor*>(parent);
	Sprite* spr = const_cast<Sprite*>(actor->GetSpr());
	spr->MountChild(actor->GetTreePath(), name, static_cast<Sprite*>(child));
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

}