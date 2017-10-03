#ifndef _SPRITE2_POLYGON_H_
#define _SPRITE2_POLYGON_H_

#include "pre_defined.h"

#include <SM_Vector.h>
#include S2_MAT_HEADER

#include <vector>
#include <memory>

namespace cooking { class DisplayList; }

namespace s2
{

class RenderColor;
class RenderParams;

class Polygon
{
public:
	virtual std::unique_ptr<Polygon> Clone() = 0;

	virtual int Type() const = 0;

	virtual void Draw(const RenderParams& rp) const = 0;
	virtual void Build() = 0;

	const std::vector<sm::vec2>& GetTriangles() const { return m_tris; }

	void SetOutline(const std::vector<sm::vec2>& outline) { m_outline = outline; }
	void SetSegments(const std::vector<sm::vec2>& segments) { m_segments = segments; }
	void SetHoles(const std::vector<std::vector<sm::vec2> >& holes) { m_holes = holes; }	

	void Clear();

	void DebugDraw(const S2_MAT& mt) const;

protected:
	std::vector<sm::vec2> m_tris;

	// shape
	std::vector<sm::vec2> m_outline;
	std::vector<sm::vec2> m_segments;
	std::vector<std::vector<sm::vec2> > m_holes;

}; // Polygon

}

#endif // _SPRITE2_POLYGON_H_