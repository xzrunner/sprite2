#ifndef _SPRITE2_TEXTURE_POLYGON_H_
#define _SPRITE2_TEXTURE_POLYGON_H_

#include "sprite2/pre_defined.h"
#include "sprite2/Polygon.h"
#include "sprite2/PolyType.h"

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

	virtual std::unique_ptr<Polygon> Clone() override {
		return std::make_unique<TexturePolygon>(*this);
	}

	virtual int Type() const override { return POLY_TEXTURE; }

	virtual void Draw(const RenderParams& rp) const override;
	virtual void Build() override;

	const CU_VEC<sm::vec2>& GetTexcoords() const { return m_texcoords; }

private:
	void GetTexBoundarySegments(const sm::rect& rect, CU_VEC<sm::vec2>& segments);

	void CalTexcoords(const sm::rect& rect);

protected:
	std::shared_ptr<const ImageSymbol> m_img = nullptr;

	CU_VEC<sm::vec2> m_texcoords;

}; // TexturePolygon

}

#endif // _SPRITE2_TEXTURE_POLYGON_H_