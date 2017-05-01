#ifndef _SPRITE2_RVG_H_
#define _SPRITE2_RVG_H_

#include <SM_Vector.h>
#include <SM_Rect.h>

#include <vector>

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
	static void Point(const sm::vec2& vertex);
	static void Points(const std::vector<sm::vec2>& vertices);

	// line
	static void Line(const sm::vec2& p0, const sm::vec2& p1);
	static void DotLine(const sm::vec2& p0, const sm::vec2& p1);
	static void DashLine(const sm::vec2& p0, const sm::vec2& p1);
	static void DotDashLine(const sm::vec2& p0, const sm::vec2& p1);

	static void Lines(const std::vector<sm::vec2>& vertices);
	static void Polyline(const std::vector<sm::vec2>& vertices, bool loop);
	static void Polyline(const float* vertices, int count, bool loop);

	// face
	static void Triangles(const std::vector<sm::vec2>& triangles);
	static void Triangles(const std::vector<sm::vec2>& triangles, const std::vector<Color>& colors);
	static void TriangleStrip(const std::vector<sm::vec2>& triangles);

	static void Rect(const sm::vec2& center, float hw, float hh, bool filling);
	static void Rect(const sm::vec2& p0, const sm::vec2& p1, bool filling);
	static void Rect(const sm::rect& r, bool filling);

	static void Circle(const sm::vec2& center, float radius, bool filling, int segments = 16);
	static void Circles(const std::vector<sm::vec2>& circles, float radius, bool filling, int segments = 16);

	// compose
	static void Cross(const sm::vec2& center, float edge);
	static void Cross(const sm::vec2& center, float xedge, float yedge);
	
	static void Arrow(const sm::vec2& p0, const sm::vec2& p1, float len = 20);

}; // RVG

}

#endif // _SPRITE2_RVG_H_