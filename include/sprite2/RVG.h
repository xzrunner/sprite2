#ifndef _SPRITE2_RVG_H_
#define _SPRITE2_RVG_H_

#include <SM_Vector.h>
#include <SM_Rect.h>
#include <cu/cu_stl.h>
#include <shaderlab/ShaderType.h>

namespace cooking { class DisplayList; }

namespace s2
{

class Color;

class RVG
{
public:
	// config
	static void Init();	

	static void SetColor(const Color& color);
	static void PointSize(float size);
	static void LineWidth(float width);

	// point
	static void Point(cooking::DisplayList* dlist, const sm::vec2& vertex);
	static void Points(cooking::DisplayList* dlist, const CU_VEC<sm::vec2>& vertices);

	// line
	static void Line(cooking::DisplayList* dlist, const sm::vec2& p0, const sm::vec2& p1);
	static void DotLine(cooking::DisplayList* dlist, const sm::vec2& p0, const sm::vec2& p1);
	static void DashLine(cooking::DisplayList* dlist, const sm::vec2& p0, const sm::vec2& p1);
	static void DotDashLine(cooking::DisplayList* dlist, const sm::vec2& p0, const sm::vec2& p1);

	static void Lines(cooking::DisplayList* dlist, const CU_VEC<sm::vec2>& vertices);
	static void Polyline(cooking::DisplayList* dlist, const CU_VEC<sm::vec2>& vertices, bool loop);
	static void Polyline(cooking::DisplayList* dlist, const float* vertices, int count, bool loop);

	// face
	static void Triangles(cooking::DisplayList* dlist, const CU_VEC<sm::vec2>& triangles);
	static void Triangles(cooking::DisplayList* dlist, const CU_VEC<sm::vec2>& triangles, const CU_VEC<Color>& colors);
	static void TriangleStrip(cooking::DisplayList* dlist, const CU_VEC<sm::vec2>& triangles);

	static void Rect(cooking::DisplayList* dlist, const sm::vec2& center, float hw, float hh, bool filling);
	static void Rect(cooking::DisplayList* dlist, const sm::vec2& p0, const sm::vec2& p1, bool filling);
	static void Rect(cooking::DisplayList* dlist, const sm::rect& r, bool filling);

	static void Circle(cooking::DisplayList* dlist, const sm::vec2& center, float radius, bool filling, int segments = 16);
	static void Circles(cooking::DisplayList* dlist, const CU_VEC<sm::vec2>& circles, float radius, bool filling, int segments = 16);

	// compose
	static void Cross(cooking::DisplayList* dlist, const sm::vec2& center, float edge);
	static void Cross(cooking::DisplayList* dlist, const sm::vec2& center, float xedge, float yedge);
	
	static void Arrow(cooking::DisplayList* dlist, const sm::vec2& p0, const sm::vec2& p1, float len = 20);

private:
	static void SetShader(cooking::DisplayList* dlist, sl::ShaderType shader);

}; // RVG

}

#endif // _SPRITE2_RVG_H_