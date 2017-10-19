#ifndef _SPRITE2_TEXTURE_POLYGON_H_
#define _SPRITE2_TEXTURE_POLYGON_H_

#include "pre_defined.h"
#include "Polygon.h"
#include "PolyType.h"

#include <SM_Rect.h>

#include <memory>

namespace s2
{

class ImageSymbol;

class TexturePolygon : public VIRTUAL_INHERITANCE Polygon
{
public:
	TexturePolygon() = default;
	TexturePolygon(const std::shared_ptr<const ImageSymbol>& img);

	virtual std::unique_ptr<Polygon> Clone() {
		return std::make_unique<TexturePolygon>(*this);
	}

	virtual int Type() const { return POLY_TEXTURE; }

	virtual void Draw(const RenderParams& rp) const;
	virtual void Build();

	const CU_VEC<sm::vec2>& GetTexcoords() const { return m_texcoords; }

private:
	void GetTexBoundarySegments(const sm::rect& rect, CU_VEC<sm::vec2>& segments);

	void CalTexcoords(const sm::rect& rect);

protected:
	std::shared_ptr<const ImageSymbol> m_img;

	CU_VEC<sm::vec2> m_texcoords;

}; // TexturePolygon

}

#endif // _SPRITE2_TEXTURE_POLYGON_H_