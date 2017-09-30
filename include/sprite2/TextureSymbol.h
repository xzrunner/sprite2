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
	virtual RenderReturn DrawTree(const RenderParams& rp, const Sprite* spr = nullptr) const;
	virtual RenderReturn DrawNode(cooking::DisplayList* dlist, const RenderParams& rp, const Sprite* spr, ft::FTList& ft, int pos) const;

	const std::vector<PolygonShape*>& GetPolygons() const { return m_polygons; }
	std::vector<PolygonShape*>& GetPolygons() { return m_polygons; }

	void AddPolygon(PolygonShape* poly);

protected:
	virtual sm::rect GetBoundingImpl(const Sprite* spr = nullptr, const Actor* actor = nullptr, bool cache = true) const;

	void Clear();

protected:
	std::vector<PolygonShape*> m_polygons;

}; // TextureSymbol

}

#endif // _SPRITE2_TEXTURE_SYMBOL_H_