#include "Particle3d.h"
#include "FastBlendMode.h"
#include "S2_Symbol.h"
#include "DrawNode.h"

#include <ps_3d.h>
#include <ps_3d_sprite.h>
#include <ps_3d_buffer.h>
#include <shaderlab.h>

#include <assert.h>

namespace s2
{

SINGLETON_DEFINITION(Particle3d)

Particle3d::Particle3d()
	: m_time(0)
{
	Init();
}

bool Particle3d::Update(float dt)
{
	m_time += dt;
	return p3d_buffer_update(m_time);
}

void Particle3d::Draw() const
{
	p3d_buffer_draw(0, 0, 1);
}

void Particle3d::BufferClear()
{
	p3d_buffer_clear();
}

static void
blend_begin_func(int blend)
{
	sl::RenderContext* ctx = sl::ShaderMgr::Instance()->GetContext();
	switch (blend)
	{
	case FBM_NULL:
		ctx->SetBlend(2, 6);		// BLEND_GL_ONE, BLEND_GL_ONE_MINUS_SRC_ALPHA
		ctx->SetBlendEquation(0);	// BLEND_FUNC_ADD
		break;
	case FBM_ADD:
		ctx->SetBlend(2, 2);		// BLEND_GL_ONE, BLEND_GL_ONE
		ctx->SetBlendEquation(0);	// BLEND_FUNC_ADD
		break;
	case FBM_SUBTRACT:
		ctx->SetBlend(2, 6);		// BLEND_GL_ONE, BLEND_GL_ONE_MINUS_SRC_ALPHA
		ctx->SetBlendEquation(1);	// BLEND_FUNC_SUBTRACT
		break;
	}
}

static void
blend_end_func()
{
	sl::RenderContext* ctx = sl::ShaderMgr::Instance()->GetContext();
	ctx->SetBlend(2, 6);		// BLEND_GL_ONE, BLEND_GL_ONE_MINUS_SRC_ALPHA
	ctx->SetBlendEquation(0);	// BLEND_FUNC_ADD
}

static void 
render_func(void* sym, float* mat, float x, float y, float angle, float scale, 
            struct ps_color* mul_col, struct ps_color* add_col, const void* ud)
{
	assert(ud);
	const P3dRenderParams* rp = (static_cast<const P3dRenderParams*>(ud));

	Symbol* s2_sym = static_cast<Symbol*>(sym);

	RenderParams params;

	params.color.mul.r = mul_col->r;
	params.color.mul.g = mul_col->g;
	params.color.mul.b = mul_col->b;
	params.color.mul.a = mul_col->a;

	params.color.add.r = add_col->r;
	params.color.add.g = add_col->g;
	params.color.add.b = add_col->b;
	params.color.add.a = add_col->a;

	params.color.mul = params.color.mul * rp->ct.mul;
	params.color.add = params.color.add + rp->ct.add;

	// todo color trans

	if (rp->p3d && rp->p3d->local_mode_draw) {
		params.mt = rp->mat;
	} else {
		sm::mat4 _mat;
		_mat.x[0] = mat[0];
		_mat.x[1] = mat[1];
		_mat.x[4] = mat[2];
		_mat.x[5] = mat[3];
		_mat.x[12]= mat[4];
		_mat.x[13]= mat[5];
		params.mt = _mat * rp->mat;
	}

	DrawNode::Draw(s2_sym, params, sm::vec2(x, y), angle, sm::vec2(scale, scale), sm::vec2(0, 0));

	// todo bind
	// 	if (p->bind_ps) {
	// 		sm::mat4 _mt;
	// 		_mt.translate(p->pos.x, p->pos.y);
	// 		Draw(p->bind_ps, _mt, recorder);
	// 	}

	// todo record
	// 	AnimRecorder* curr_record = m_anim_recorder ? m_anim_recorder : recorder;
	// 	if (curr_record) {
	// 		sm::vec2 fixed = _mt * pos;
	// 		curr_record->AddItem(sym->GetFilepath(), fixed.x, fixed.y, p->angle, s, mul_col, add_col);
	// 	}
}

static void 
add_func(p3d_particle* p, void* ud)
{
//  	ParticleSystem* ps = (ParticleSystem*)ud;
//  	const sm::vec2& pos = ps->GetPosition();
//  	p->init_pos.x = pos.x;
//  	p->init_pos.y = pos.y;
}

static void 
remove_func(p3d_particle* p, void* ud)
{
// 	ParticleSystem* ps = (ParticleSystem*)ud;
// 	if (ps) {
// 		ps->RemoveFromInvertRecord(p);
// 	}
}

static void
update_srt_func(void* params, float x, float y, float scale) {
	P3dRenderParams* rp = static_cast<P3dRenderParams*>(params);
	rp->mat.Translate(x, y, 0);
	rp->mat.Scale(scale, scale, 1);
}

static void
buf_remove_func(struct p3d_sprite* spr) {
}

static void
create_draw_params_func(struct p3d_sprite* spr) {
	P3dRenderParams* rp = new P3dRenderParams;
	rp->p3d = spr;
	spr->draw_params = rp;
}

void 
release_draw_params_func(struct p3d_sprite* spr) {
	P3dRenderParams* rp = static_cast<P3dRenderParams*>(spr->draw_params);
	delete rp;
	spr->draw_params = NULL;
}

void Particle3d::Init()
{
	p3d_init();
	p3d_regist_cb(blend_begin_func, blend_end_func, render_func, add_func, remove_func);	
	p3d_buffer_init(update_srt_func, buf_remove_func);
	p3d_sprite_init(create_draw_params_func, release_draw_params_func);
}

}