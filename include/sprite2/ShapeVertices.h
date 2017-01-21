#ifndef _SPRITE2_SHAPE_VERTICES_H_
#define _SPRITE2_SHAPE_VERTICES_H_

#include "pre_defined.h"

#include <SM_Vector.h>
#include S2_MAT_HEADER

#include <vector>

namespace s2
{

class RenderColor;

class ShapeVertices
{
public:
	static void Draw(const std::vector<sm::vec2>& vertices, bool closed,
		const S2_MAT& mt, const RenderColor& color);

}; // ShapeVertices

}

#endif // _SPRITE2_SHAPE_VERTICES_H_