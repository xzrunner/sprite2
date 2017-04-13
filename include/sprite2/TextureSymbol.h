#ifndef _SPRITE2_TEXTURE_SYMBOL_H_
#define _SPRITE2_TEXTURE_SYMBOL_H_

#include "S2_Symbol.h"

#include <vector>

#include <stdint.h>

namespace s2
{

class PolygonShape;

class TextureSymbol : public VIRTUAL_INHERITANCE Symbol
{
public:
	TextureSymbol();
	TextureSymbol(uint32_t id);
	virtual ~TextureSymbol();

	/**
	 *  @interface
	 *    Symbol
	 */
	virtual int Type() const;
	virtual void Traverse(const SymbolVisitor& visitor) {}
	virtual void Draw(const RenderParams& rp, const Sprite* spr = NULL) const;
	virtual sm::rect GetBounding(const Sprite* spr = NULL, const Actor* actor = NULL, bool cache = true) const;

	const std::vector<PolygonShape*>& GetPolygons() const { return m_polygons; }
	std::vector<PolygonShape*>& GetPolygons() { return m_polygons; }

	void AddPolygon(PolygonShape* poly);

protected:
	void Clear();

protected:
	std::vector<PolygonShape*> m_polygons;

}; // TextureSymbol

}

#endif // _SPRITE2_TEXTURE_SYMBOL_H_