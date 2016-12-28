#ifndef _SPRITE2_DRAW_MESH_H_
#define _SPRITE2_DRAW_MESH_H_

#include <SM_Matrix.h>

namespace s2
{

class Mesh;
class RenderParams;
class Symbol;

class DrawMesh
{
public:
	static void DrawInfoUV(const Mesh* mesh, const sm::mat4* mt = NULL);
	static void DrawInfoXY(const Mesh* mesh, const sm::mat4* mt = NULL);

	static void DrawTexture(const Mesh* mesh, const RenderParams& params, 
		const Symbol* base_sym = NULL);

	static void DrawOnlyMesh(const Mesh* mesh, const sm::mat4& mt, int texid);

private:
	static void DrawPass1(const Mesh* mesh, const RenderParams& params, const Symbol* sym);

	static void DrawPass2(const Mesh* mesh, const RenderParams& params, const Symbol* sym);
	static void DrawMeshToTmp(const RenderParams& params, const Symbol* sym);
	static void DrawTmpToScreen(const Mesh* mesh, const sm::mat4& mt);

}; // DrawMesh

}

#endif // _SPRITE2_DRAW_MESH_H_