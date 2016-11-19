#include "Animation2.h"
#include "DrawNode.h"
#include "S2_Symbol.h"
#include "SymType.h"
#include "MeshSymbol.h"

#include <rigging/rg_skeleton.h>
#include <rigging/rg_skeleton_skin.h>
#include <rigging/rg_timeline.h>

namespace s2
{

SINGLETON_DEFINITION(Animation2)

Animation2::Animation2()
{
	Init();
}

static void
render_func(void* sym, float* mat, const void* ud) 
{
	Symbol* s2_sym = static_cast<Symbol*>(sym);
	const RenderParams* params = static_cast<const RenderParams*>(ud);
	sm::mat4 m;

	m.c[0][0] = mat[0]; m.c[0][1] = mat[1];
	m.c[1][0] = mat[2]; m.c[1][1] = mat[3];
	m.c[3][0] = mat[4]; m.c[3][1] = mat[5];

	DrawNode::Draw(s2_sym, *params, m);
}

static void
update_skin_func(void* sym, const rg_skeleton_pose* sk_pose) 
{
	Symbol* s2_sym = static_cast<Symbol*>(sym);
	if (s2_sym->Type() == s2::SYM_MESH) {
		MeshSymbol* mesh_sym = VI_DOWNCASTING<MeshSymbol*>(s2_sym);
		mesh_sym->UpdateMesh(sk_pose);
	}
}

static void
update_mesh_func(void* sym, const rg_tl_deform_state* deform_state, const float* vertices) 
{
	Symbol* s2_sym = static_cast<Symbol*>(sym);
	if (s2_sym->Type() == s2::SYM_MESH) {
		MeshSymbol* mesh_sym = VI_DOWNCASTING<MeshSymbol*>(s2_sym);
		mesh_sym->UpdateMesh(deform_state, vertices);
	}
}

void Animation2::Init()
{
	rg_skeleton_init(render_func);
	rg_skeleton_skin_init(update_skin_func, update_mesh_func);
	rg_timeline_init();
}

}