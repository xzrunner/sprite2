#ifndef _SPRITE2_DRAW_MESH_H_
#define _SPRITE2_DRAW_MESH_H_

#include "pre_defined.h"
#include "RenderReturn.h"

#include S2_MAT_HEADER

#include <stddef.h>

namespace s2
{

class Mesh;
class RenderParams;
class Symbol;
class RenderTarget;

class DrawMesh
{
public:
	static RenderReturn DrawInfoUV(const Mesh* mesh, const S2_MAT* mt = nullptr);
	static RenderReturn DrawInfoXY(const Mesh* mesh, const S2_MAT* mt = nullptr);

	static RenderReturn DrawTexture(const Mesh* mesh, const RenderParams& rp, 
		const Symbol* base_sym = nullptr);

	static RenderReturn DrawOnlyMesh(const Mesh* mesh, const S2_MAT& mt, int tex_id);

private:
	static RenderReturn DrawOnePass(const Mesh* mesh, const RenderParams& rp, const float* texcoords, int tex_id);

	static RenderReturn DrawTwoPass(const Mesh* mesh, const RenderParams& rp, const Symbol* sym);
	static RenderReturn DrawMesh2RT(RenderTarget* rt, const RenderParams& rp, const Symbol* sym);
	static RenderReturn DrawRT2Screen(RenderTarget* rt, const Mesh* mesh, const S2_MAT& mt);

}; // DrawMesh

}

#endif // _SPRITE2_DRAW_MESH_H_