#include "Particle3d.h"
#include "Particle3dBuffer.h"
#include "FastBlendMode.h"
#include "S2_Symbol.h"
#include "S2_Sprite.h"
#include "DrawNode.h"
#include "SymType.h"
#include "UpdateParams.h"
#include "P3dRenderParams.h"

#include "TrailSymbol.h"
#include "TrailSprite.h"
#include "Particle3dSymbol.h"
#include "Particle3dSprite.h"

#include <ps_3d.h>
#include <unirender/UR_RenderContext.h>
#include <shaderlab/ShaderMgr.h>

#include <assert.h>
#include <string.h>

namespace s2
{

SINGLETON_DEFINITION(Particle3d)

Particle3d::Particle3d()
	: m_time(0)
{
	Init();
}

bool Particle3d::BufferUpdate(float dt)
{
	m_time += dt;
	return Particle3dBuffer::Instance()->Update(m_time);
}

void Particle3d::BufferDraw(float x, float y, float scale) const
{
	Particle3dBuffer::Instance()->Draw(sm::vec2(x, y), scale);
}

void Particle3d::BufferClear()
{
	Particle3dBuffer::Instance()->Clear();
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
            struct ps_color* mul_col, struct ps_color* add_col, int fast_blend, const void* ud, 
		    float time)
{
	assert(ud);
	const P3dRenderParams* rp = (static_cast<const P3dRenderParams*>(ud));

	Color mul, add;
	memcpy(&mul, mul_col, sizeof(mul));
	memcpy(&add, add_col, sizeof(add));

	RenderParams* rp_child = RenderParamsPool::Instance()->Pop();
	rp_child->Reset();

	if (rp->flags != 0xffffffff) {
		rp_child->SetFlags(rp->flags);
	}

	rp_child->color.SetMul(mul * rp->rc.GetMul());
	rp_child->color.SetAdd(add + rp->rc.GetAdd());

	rp_child->shader.SetFastBlend(static_cast<FastBlendMode>(fast_blend));

	rp_child->SetViewRegion(rp->view_region);

	// todo color trans

	if (rp->local) {
		// local mode, use node's mat
		rp_child->mt = rp->mt;
	} else {
		// no local mode, use particle's mat
#ifdef S2_MATRIX_FIX
		sm::MatrixFix _mat;
		_mat.x[0] = mat[0] * sm::MatrixFix::SCALE;
		_mat.x[1] = mat[1] * sm::MatrixFix::SCALE;
		_mat.x[2] = mat[2] * sm::MatrixFix::SCALE;
		_mat.x[3] = mat[3] * sm::MatrixFix::SCALE;
		_mat.x[4] = mat[4] * sm::MatrixFix::TRANSLATE_SCALE;
		_mat.x[5] = mat[5] * sm::MatrixFix::TRANSLATE_SCALE;
#else
		sm::Matrix2D _mat;
		memcpy(_mat.x, mat, sizeof(_mat.x));
#endif // S2_MATRIX_FIX
		rp_child->mt = _mat;
	}

	if (spr) {
		Sprite* s2_spr = static_cast<Sprite*>(spr);
		DrawNode::Draw(s2_spr, *rp_child);
	} else if (sym) {
		Symbol* s2_sym = static_cast<Symbol*>(sym);
		DrawNode::Draw(s2_sym, *rp_child, sm::vec2(x, y), angle, sm::vec2(scale, scale), sm::vec2(0, 0));
		s2_sym->Update(UpdateParams(), time);
	}

	// todo record
	// 	AnimRecorder* curr_record = m_anim_recorder ? m_anim_recorder : recorder;
	// 	if (curr_record) {
	// 		sm::vec2 fixed = _mt * pos;
	// 		curr_record->AddItem(sym->GetFilepath(), fixed.x, fixed.y, p->angle, s, mul_col, add_col);
	// 	}

	RenderParamsPool::Instance()->Push(rp_child); 
}

static void
update_func(void* spr, float x, float y)
{
	if (!spr) {
		return;
	}

	Sprite* s2_spr = static_cast<Sprite*>(spr);
	UpdateParams up;
	S2_MAT mat;
	mat.Translate(x, y);
	up.SetPrevMat(mat);
	s2_spr->Update(up);
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
		p->ud = nullptr;
	}
}

void Particle3d::Init()
{
	p3d_init();
	p3d_regist_cb(blend_begin_func, blend_end_func, render_func, update_func, add_func, remove_func);	
}

void Particle3d::Tick()
{
	p3d_tick();
}

void Particle3d::GC()
{
	p3d_gc();
}

}
