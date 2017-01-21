#ifndef _SPRITE2_DRAW_MESH_H_
#define _SPRITE2_DRAW_MESH_H_

#include "pre_defined.h"

#include S2_MAT_HEADER

namespace s2
{

class Mesh;
class RenderParams;
class Symbol;
class RenderTarget;

class DrawMesh
{
public:
	static void DrawInfoUV(const Mesh* mesh, const S2_MAT* mt = NULL);
	static void DrawInfoXY(const Mesh* mesh, const S2_MAT* mt = NULL);

	static void DrawTexture(const Mesh* mesh, const RenderParams& params, 
		const Symbol* base_sym = NULL);

	static void DrawOnlyMesh(const Mesh* mesh, const S2_MAT& mt, int texid);

private:
	static void DrawOnePass(const Mesh* mesh, const RenderParams& params, const Symbol* sym);

	static void DrawTwoPass(const Mesh* mesh, const RenderParams& params, const Symbol* sym);
	static void DrawMesh2RT(RenderTarget* rt, const RenderParams& params, const Symbol* sym);
	static void DrawRT2Screen(RenderTarget* rt, const Mesh* mesh, const S2_MAT& mt);

}; // DrawMesh

}

#endif // _SPRITE2_DRAW_MESH_H_