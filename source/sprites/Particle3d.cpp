#include "Particle3d.h"
#include "FastBlendMode.h"
#include "S2_Symbol.h"
#include "S2_Sprite.h"
#include "DrawNode.h"
#include "SymType.h"

#include "TrailSymbol.h"
#include "TrailSprite.h"
#include "Particle3dSymbol.h"
#include "Particle3dSprite.h"

#include <ps_3d.h>
#include <ps_3d_sprite.h>
#include <ps_3d_buffer.h>
#include <unirender/RenderContext.h>
#include <shaderlab/ShaderMgr.h>

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
	ur::RenderContext* rc = sl::ShaderMgr::Instance()->GetContext();
	switch (blend)
	{
	case FBM_NULL:
		rc->SetBlend(2, 6);		// BLEND_GL_ONE, BLEND_GL_ONE_MINUS_SRC_ALPHA
		rc->SetBlendEquation(0);	// BLEND_FUNC_ADD
		break;
	case FBM_ADD:
		rc->SetBlend(2, 2);		// BLEND_GL_ONE, BLEND_GL_ONE
		rc->SetBlendEquation(0);	// BLEND_FUNC_ADD
		break;
	case FBM_SUBTRACT:
		rc->SetBlend(2, 6);		// BLEND_GL_ONE, BLEND_GL_ONE_MINUS_SRC_ALPHA
		rc->SetBlendEquation(1);	// BLEND_FUNC_SUBTRACT
		break;
	}
}

static void
blend_end_func()
{
	ur::RenderContext* rc = sl::ShaderMgr::Instance()->GetContext();
	rc->SetBlend(2, 6);		// BLEND_GL_ONE, BLEND_GL_ONE_MINUS_SRC_ALPHA
	rc->SetBlendEquation(0);	// BLEND_FUNC_ADD
}

static void 
render_func(void* spr, void* sym, float* mat, float x, float y, float angle, float scale, 
            struct ps_color* mul_col, struct ps_color* add_col, int fast_blend, const void* ud, float time)
{
	assert(ud);
	const P3dRenderParams* rp = (static_cast<const P3dRenderParams*>(ud));

	Color mul, add;
	memcpy(&mul, mul_col, sizeof(mul));
	memcpy(&add, add_col, sizeof(add));

	RenderParams rp_child;

	rp_child.color.SetMul(mul * rp->ct.GetMul());
	rp_child.color.SetAdd(add + rp->ct.GetAdd());

	rp_child.shader.SetFastBlend(static_cast<FastBlendMode>(fast_blend));

	// todo color trans

	if (rp->p3d && rp->p3d->local_mode_draw) {
		rp_child.mt = rp->mat;
	} else {
#ifdef S2_MATRIX_FIX
		sm::MatrixFix _mat;
		_mat.x[0] = mat[0] * sm::MatrixFix::SCALE;
		_mat.x[1] = mat[1] * sm::MatrixFix::SCALE;
		_mat.x[2] = mat[2] * sm::MatrixFix::SCALE;
		_mat.x[3] = mat[3] * sm::MatrixFix::SCALE;
		_mat.x[4] = mat[4] * sm::MatrixFix::TRANSLATE_SCALE;
		_mat.x[5] = mat[5] * sm::MatrixFix::TRANSLATE_SCALE;
#else
		sm::mat4 _mat;
		_mat.x[0] = mat[0];
		_mat.x[1] = mat[1];
		_mat.x[4] = mat[2];
		_mat.x[5] = mat[3];
		_mat.x[12]= mat[4];
		_mat.x[13]= mat[5];
#endif // S2_MATRIX_FIX
		rp_child.mt = _mat * rp->mat;
	}

	if (spr) {
		Sprite* s2_spr = static_cast<Sprite*>(spr);
		DrawNode::Draw(s2_spr, rp_child);
	} else if (sym) {
		Symbol* s2_sym = static_cast<Symbol*>(sym);
		DrawNode::Draw(s2_sym, rp_child, sm::vec2(x, y), angle, sm::vec2(scale, scale), sm::vec2(0, 0));
		s2_sym->Update(rp_child, time);
	}

	// todo record
	// 	AnimRecorder* curr_record = m_anim_recorder ? m_anim_recorder : recorder;
	// 	if (curr_record) {
	// 		sm::vec2 fixed = _mt * pos;
	// 		curr_record->AddItem(sym->GetFilepath(), fixed.x, fixed.y, p->angle, s, mul_col, add_col);
	// 	}
}

static void
update_func(void* spr, float x, float y)
{
	if (!spr) {
		return;
	}

	Sprite* s2_spr = static_cast<Sprite*>(spr);
	RenderParams rp;
#ifdef S2_MATRIX_FIX
	rp.mt.Translate(x, y);
#else
	rp.mt = sm::mat4::Translated(x, y, 0);
#endif // S2_MATRIX_FIX
	s2_spr->Update(rp);
}

static void 
add_func(p3d_particle* p, void* ud)
{
//  	ParticleSystem* ps = (ParticleSystem*)ud;
//  	const sm::vec2& pos = ps->GetPosition();
//  	p->init_pos.x = pos.x;
//  	p->init_pos.y = pos.y;

	//////////////////////////////////////////////////////////////////////////

	if (!p->ud) {
		Symbol* sym = static_cast<Symbol*>(p->cfg.sym->ud);
		switch (sym->Type())
		{
		case SYM_TRAIL:
			{
				TrailSprite* s2_spr = new TrailSprite(sym);
				s2_spr->SetInP3d();
				p->ud = static_cast<Sprite*>(s2_spr);
			}
			break;
		case SYM_PARTICLE3D:
			{
				Sprite* s2_p3d = new Particle3dSprite(sym);
				p->ud = s2_p3d;
			}
			break;
		}
	}
}

static void 
remove_func(p3d_particle* p, void* ud)
{
// 	ParticleSystem* ps = (ParticleSystem*)ud;
// 	if (ps) {
// 		ps->RemoveFromInvertRecord(p);
// 	}

	//////////////////////////////////////////////////////////////////////////

	if (p->ud) {
		Sprite* spr = static_cast<Sprite*>(p->ud);
		spr->RemoveReference();
		p->ud = NULL;
	}
}

static void
update_srt_func(void* params, float x, float y, float scale) {
	P3dRenderParams* rp = static_cast<P3dRenderParams*>(params);
#ifdef S2_MATRIX_FIX
	rp->mat.Translate(x, y);
	rp->mat.Scale(scale, scale);
#else
	rp->mat.Translate(x, y, 0);
	rp->mat.Scale(scale, scale, 1);
#endif // S2_MATRIX_FIX
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
	p3d_regist_cb(blend_begin_func, blend_end_func, render_func, update_func, add_func, remove_func);	
	p3d_buffer_init(update_srt_func, buf_remove_func);
	p3d_sprite_init(create_draw_params_func, release_draw_params_func);
}

}