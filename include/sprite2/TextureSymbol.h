#ifndef _SPRITE2_TEXTURE_SYMBOL_H_
#define _SPRITE2_TEXTURE_SYMBOL_H_

#include "S2_Symbol.h"
#include "PolygonShape.h"

#include <vector>

#include <stdint.h>

namespace s2
{

class TextureSymbol : public VIRTUAL_INHERITANCE Symbol
{
public:
	TextureSymbol();
	TextureSymbol(uint32_t id);
	TextureSymbol(const TextureSymbol&);
	TextureSymbol& operator = (const TextureSymbol&);
	virtual ~TextureSymbol();

	/**
	 *  @interface
	 *    Symbol
	 */
	virtual int Type() const;
	virtual void Traverse(const SymbolVisitor& visitor) {}
	virtual RenderReturn DrawTree(const RenderParams& rp, const SprConstPtr& spr = nullptr) const;
	virtual RenderReturn DrawNode(cooking::DisplayList* dlist, const RenderParams& rp, const SprConstPtr& spr, ft::FTList& ft, int pos) const;

	const std::vector<std::unique_ptr<PolygonShape>>& GetPolygons() const { return m_polygons; }
	std::vector<std::unique_ptr<PolygonShape>>& GetPolygons() { return m_polygons; }

	void AddPolygon(std::unique_ptr<PolygonShape>& poly) {
		m_polygons.push_back(std::move(poly));
	}

protected:
	virtual sm::rect GetBoundingImpl(const SprConstPtr& spr = nullptr, const ActorConstPtr& actor = nullptr, bool cache = true) const;

	void Clear();

protected:
	std::vector<std::unique_ptr<PolygonShape>> m_polygons;

}; // TextureSymbol

}

#endif // _SPRITE2_TEXTURE_SYMBOL_H_