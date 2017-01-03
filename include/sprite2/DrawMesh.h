#ifndef _SPRITE2_DRAW_MESH_H_
#define _SPRITE2_DRAW_MESH_H_

#include <SM_Matrix.h>

namespace s2
{

class Mesh;
class RenderParams;
class Symbol;
class RenderTarget;

class DrawMesh
{
public:
	static void DrawInfoUV(const Mesh* mesh, const sm::mat4* mt = NULL);
	static void DrawInfoXY(const Mesh* mesh, const sm::mat4* mt = NULL);

	static void DrawTexture(const Mesh* mesh, const RenderParams& params, 
		const Symbol* base_sym = NULL);

	static void DrawOnlyMesh(const Mesh* mesh, const sm::mat4& mt, int texid);

private:
	static void DrawOnePass(const Mesh* mesh, const RenderParams& params, const Symbol* sym);

	static void DrawTwoPass(const Mesh* mesh, const RenderParams& params, const Symbol* sym);
	static void DrawMesh2RT(RenderTarget* rt, const RenderParams& params, const Symbol* sym);
	static void DrawRT2Screen(RenderTarget* rt, const Mesh* mesh, const sm::mat4& mt);

}; // DrawMesh

}

#endif // _SPRITE2_DRAW_MESH_H_