#include "Animation2.h"
#include "DrawNode.h"
#include "S2_Symbol.h"
#include "SymType.h"
#include "MeshSymbol.h"
#include "S2_RVG.h"

#include <rigging.h>

#include <string.h>

namespace s2
{

CU_SINGLETON_DEFINITION(Animation2)

Animation2::Animation2()
{
	Init();
}

static void
render_func(void* sym, float* mat, const void* ud) 
{
	SymPtr s2_sym(static_cast<Symbol*>(sym));

	const RenderParams* params = static_cast<const RenderParams*>(ud);

#ifdef S2_MATRIX_FIX
	sm::MatrixFix m;	
	m.x[0] = mat[0] * sm::MatrixFix::SCALE;
	m.x[1] = mat[1] * sm::MatrixFix::SCALE;
	m.x[2] = mat[2] * sm::MatrixFix::SCALE;
	m.x[3] = mat[3] * sm::MatrixFix::SCALE;
	m.x[4] = mat[4] * sm::MatrixFix::TRANSLATE_SCALE;
	m.x[5] = mat[5] * sm::MatrixFix::TRANSLATE_SCALE;
#else
	sm::Matrix2D m;
	memcpy(m.x, mat, sizeof(m.x));
#endif // S2_MATRIX_FIX

	DrawNode::Draw(s2_sym, *params, m);
}

static void
debug_draw_func(float x, float y, uint32_t color)
{
	Color col;
	col.FromRGBA(color);
	RVG::SetColor(col);
	RVG::Rect(sm::vec2(x, y), 5, 5, true);
}

static void
update_skin_func(void* sym, const rg_skeleton_pose* sk_pose) 
{
	SymPtr s2_sym(static_cast<Symbol*>(sym));
	if (s2_sym->Type() == SYM_MESH) {
		S2_VI_PTR_DOWN_CAST<MeshSymbol>(s2_sym)->UpdateMesh(sk_pose);
	}
}

static void
update_mesh_func(void* sym, const rg_tl_deform_state* deform_state, const float* vertices) 
{
	SymPtr s2_sym(static_cast<Symbol*>(sym));
	if (s2_sym->Type() == SYM_MESH) {
		S2_VI_PTR_DOWN_CAST<MeshSymbol>(s2_sym)->UpdateMesh(deform_state, vertices);
	}
}

void Animation2::Init()
{
	rg_skeleton_init(render_func, debug_draw_func);
	rg_skeleton_skin_init(update_skin_func, update_mesh_func);
	rg_timeline_init();
}

}