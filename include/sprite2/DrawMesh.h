#ifndef _SPRITE2_DRAW_MESH_H_
#define _SPRITE2_DRAW_MESH_H_

#include "sprite2/pre_defined.h"
#include "sprite2/RenderReturn.h"
#include "sprite2/typedef.h"

#include S2_MAT_HEADER

#include <memory>

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
	static RenderReturn DrawInfoUV(const std::unique_ptr<Mesh>& mesh, const S2_MAT* mt = nullptr);
	static RenderReturn DrawInfoXY(const std::unique_ptr<Mesh>& mesh, const S2_MAT* mt = nullptr);

	static RenderReturn DrawTexture(const std::unique_ptr<Mesh>& mesh, const RenderParams& rp, 
		const SymConstPtr& base_sym = nullptr);

	static RenderReturn DrawOnlyMesh(const std::unique_ptr<Mesh>& mesh, const S2_MAT& mt, int tex_id);

private:
	static RenderReturn DrawOnePass(const std::unique_ptr<Mesh>& mesh, const RenderParams& rp, const float* texcoords, int tex_id);

	static RenderReturn DrawTwoPass(const std::unique_ptr<Mesh>& mesh, const RenderParams& rp, const Symbol& sym);
	static RenderReturn DrawMesh2RT(RenderTarget* rt, const RenderParams& rp, const Symbol& sym);
	static RenderReturn DrawRT2Screen(RenderTarget* rt, const std::unique_ptr<Mesh>& mesh, const S2_MAT& mt);

}; // DrawMesh

}

#endif // _SPRITE2_DRAW_MESH_H_