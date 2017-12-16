#ifndef _SPRITE2_SHAPE_VERTICES_H_
#define _SPRITE2_SHAPE_VERTICES_H_

#include "sprite2/pre_defined.h"

#include <SM_Vector.h>
#include S2_MAT_HEADER

#include <cu/cu_stl.h>

namespace cooking { class DisplayList; }

namespace s2
{

class RenderParams;

class ShapeVertices
{
public:
	static void Draw(cooking::DisplayList* dlist, const CU_VEC<sm::vec2>& vertices, 
		bool closed, const RenderParams& rp);

}; // ShapeVertices

}

#endif // _SPRITE2_SHAPE_VERTICES_H_