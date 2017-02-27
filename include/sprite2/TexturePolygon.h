#ifndef _SPRITE2_TEXTURE_POLYGON_H_
#define _SPRITE2_TEXTURE_POLYGON_H_

#include "pre_defined.h"
#include "Polygon.h"

#include <SM_Rect.h>

namespace s2
{

class ImageSymbol;

class TexturePolygon : public VIRTUAL_INHERITANCE Polygon
{
public:
	TexturePolygon();
	TexturePolygon(const TexturePolygon& poly);
	TexturePolygon& operator = (const TexturePolygon& poly);
	TexturePolygon(const ImageSymbol* img);
	virtual ~TexturePolygon();

	virtual void Draw(const RenderParams& rp) const;
	virtual void Build();

	const std::vector<sm::vec2>& GetTexcoords() const { return m_texcoords; }

private:
	void GetTexBoundarySegments(const sm::rect& rect, std::vector<sm::vec2>& segments);

	void CalTexcoords(const sm::rect& rect);

protected:
	ImageSymbol* m_img;

	std::vector<sm::vec2> m_texcoords;

}; // TexturePolygon

}

#endif // _SPRITE2_TEXTURE_POLYGON_H_