#ifndef _SPRITE2_COLOR_POLYGON_H_
#define _SPRITE2_COLOR_POLYGON_H_

#include "sprite2/pre_defined.h"
#include "sprite2/Polygon.h"
#include "sprite2/PolyType.h"

#include <painting2/Color.h>

namespace s2
{

class ColorPolygon : public VIRTUAL_INHERITANCE Polygon
{
public:
	ColorPolygon() {}
	ColorPolygon(const pt2::Color& color);
	
	virtual std::unique_ptr<Polygon> Clone() override {
		return std::make_unique<ColorPolygon>(*this);
	}

	virtual int Type() const override { return POLY_COLOR; }

	virtual void Draw(cooking::DisplayList* dlist, const RenderParams& rp) const override;
	virtual void Build() override;

	const pt2::Color& GetColor() const { return m_color; }

protected:
	pt2::Color m_color;

}; // ColorPolygon

}

#endif // _SPRITE2_COLOR_POLYGON_H_