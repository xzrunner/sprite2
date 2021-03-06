#include "sprite2/Particle2d.h"
#include "sprite2/Symbol.h"
#include "sprite2/RenderParams.h"
#include "sprite2/DrawNode.h"

#include S2_MAT_HEADER
#include <ps_2d.h>
#include <painting2/Color.h>

#include <string.h>

namespace s2
{

CU_SINGLETON_DEFINITION(Particle2d)

Particle2d::Particle2d()
	: m_time(0)
{
	Init();
}

void Particle2d::Update(float dt)
{
	m_time += dt;
}

static void 
render_func(void* sym, float* mat, float x, float y, float angle, float scale, 
            struct ps_color* mul_col, struct ps_color* add_col, const void* ud)
{
	S2_MAT mt;
	if (ud) {
		mt = *(S2_MAT*)ud;
	} else {
#ifdef S2_MATRIX_FIX
		mt.x[0] = mat[0] * sm::MatrixFix::SCALE;
		mt.x[1] = mat[1] * sm::MatrixFix::SCALE;
		mt.x[2] = mat[2] * sm::MatrixFix::SCALE;
		mt.x[3] = mat[3] * sm::MatrixFix::SCALE;
		mt.x[4] = mat[4] * sm::MatrixFix::TRANSLATE_SCALE;
		mt.x[5] = mat[5] * sm::MatrixFix::TRANSLATE_SCALE;
#else
		memcpy(mt.x, mat, sizeof(mt.x));
#endif // S2_MATRIX_FIX
	}

	Symbol* s2_sym(static_cast<Symbol*>(sym));

	pt2::Color mul, add;
	memcpy(&mul, mul_col, sizeof(*mul_col));
	memcpy(&add, add_col, sizeof(*add_col));

	RenderParams rp;
	rp.mt = mt;
	rp.col_common.mul = mul;
	rp.col_common.add = add;

	DrawNode::Draw(*s2_sym, rp, sm::vec2(x, y), angle, sm::vec2(scale, scale), sm::vec2(0, 0));
}

void Particle2d::Init()
{
	p2d_init();
	p2d_regist_cb(render_func);	
}

}