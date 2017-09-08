#ifndef _SPRITE2_COLOR_POLYGON_H_
#define _SPRITE2_COLOR_POLYGON_H_

#include "pre_defined.h"
#include "Polygon.h"
#include "PolyType.h"
#include "Color.h"

namespace s2
{

class Color;

class ColorPolygon : public VIRTUAL_INHERITANCE Polygon
{
public:
	ColorPolygon() {}
	ColorPolygon(const Color& color);
	
	virtual int Type() const { return POLY_COLOR; }

	virtual void Draw(const RenderParams& rp) const;
	virtual void DrawDeferred(cooking::DisplayList* dlist, const RenderParams& rp) const;
	virtual void Build();

	const Color& GetColor() const { return m_color; }

protected:
	Color m_color;

}; // ColorPolygon

}

#endif // _SPRITE2_COLOR_POLYGON_H_