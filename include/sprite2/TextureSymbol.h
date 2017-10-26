#ifndef _SPRITE2_TEXTURE_SYMBOL_H_
#define _SPRITE2_TEXTURE_SYMBOL_H_

#include "sprite2/Symbol.h"
#include "sprite2/PolygonShape.h"

#include <cu/cu_stl.h>

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
	virtual int Type() const override;
	virtual void Traverse(const SymbolVisitor& visitor) override {}
	virtual RenderReturn DrawTree(cooking::DisplayList* dlist, const RenderParams& rp, const Sprite* spr = nullptr) const override;
#ifndef S2_DISABLE_FLATTEN
	virtual RenderReturn DrawNode(cooking::DisplayList* dlist, const RenderParams& rp, const Sprite* spr, ft::FTList& ft, int pos) const override;
#endif // S2_DISABLE_FLATTEN

	const CU_VEC<std::unique_ptr<PolygonShape>>& GetPolygons() const { return m_polygons; }
	CU_VEC<std::unique_ptr<PolygonShape>>& GetPolygons() { return m_polygons; }

	void AddPolygon(std::unique_ptr<PolygonShape>& poly) {
		m_polygons.push_back(std::move(poly));
	}

protected:
	virtual sm::rect GetBoundingImpl(const Sprite* spr = nullptr, const Actor* actor = nullptr, bool cache = true) const;

	void Clear();

protected:
	CU_VEC<std::unique_ptr<PolygonShape>> m_polygons;

}; // TextureSymbol

}

#endif // _SPRITE2_TEXTURE_SYMBOL_H_