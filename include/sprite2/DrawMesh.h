#ifndef _SPRITE2_DRAW_MESH_H_
#define _SPRITE2_DRAW_MESH_H_

#include "sprite2/pre_defined.h"
#include "sprite2/RenderReturn.h"
#include "sprite2/typedef.h"

#include S2_MAT_HEADER

#include <stddef.h>

namespace cooking { class DisplayList; }
namespace pt2 { class RenderTarget; }

namespace s2
{

class Mesh;
class RenderParams;
class Symbol;

class DrawMesh
{
public:
	static RenderReturn DrawInfoUV(cooking::DisplayList* dlist, const Mesh& mesh, const S2_MAT* mt = nullptr);
	static RenderReturn DrawInfoXY(cooking::DisplayList* dlist, const Mesh& mesh, const S2_MAT* mt = nullptr);

	static RenderReturn DrawTexture(cooking::DisplayList* dlist, const Mesh& mesh, 
		const RenderParams& rp, const SymConstPtr& base_sym = nullptr);

	static RenderReturn DrawOnlyMesh(cooking::DisplayList* dlist, const Mesh& mesh, const S2_MAT& mt, int tex_id);

private:
	static RenderReturn DrawOnePass(cooking::DisplayList* dlist, const Mesh& mesh, const RenderParams& rp, const float* texcoords, int tex_id);

	static RenderReturn DrawTwoPass(cooking::DisplayList* dlist, const Mesh& mesh, const RenderParams& rp, const Symbol& sym);
	static RenderReturn DrawMesh2RT(cooking::DisplayList* dlist, pt2::RenderTarget* rt, const RenderParams& rp, const Symbol& sym);
	static RenderReturn DrawRT2Screen(cooking::DisplayList* dlist, pt2::RenderTarget* rt, const Mesh& mesh, const S2_MAT& mt);

}; // DrawMesh

}

#endif // _SPRITE2_DRAW_MESH_H_