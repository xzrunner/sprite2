#ifndef _SPRITE2_COLOR_POLYGON_H_
#define _SPRITE2_COLOR_POLYGON_H_

#include "sprite2/pre_defined.h"
#include "sprite2/Polygon.h"
#include "sprite2/PolyType.h"
#include "sprite2/Color.h"

namespace s2
{

class Color;

class ColorPolygon : public VIRTUAL_INHERITANCE Polygon
{
public:
	ColorPolygon() {}
	ColorPolygon(const Color& color);
	
	virtual std::unique_ptr<Polygon> Clone() override {
		return std::make_unique<ColorPolygon>(*this);
	}

	virtual int Type() const override { return POLY_COLOR; }

	virtual void Draw(const RenderParams& rp) const override;
	virtual void Build() override;

	const Color& GetColor() const { return m_color; }

protected:
	Color m_color;

}; // ColorPolygon

}

#endif // _SPRITE2_COLOR_POLYGON_H_