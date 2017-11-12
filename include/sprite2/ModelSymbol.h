#ifndef _SPRITE2_MODEL_SYMBOL_H_
#define _SPRITE2_MODEL_SYMBOL_H_

#include "sprite2/config.h"

#ifndef S2_DISABLE_MODEL

#include "sprite2/Symbol.h"

#include <model3/AABB.h>

namespace m3 { class Model; }

namespace s2
{

class ModelSymbol : public VIRTUAL_INHERITANCE Symbol
{
public:
	ModelSymbol();
	virtual ~ModelSymbol();

	/**
	 *  @interface
	 *    Symbol
	 */
	virtual int Type() const override;
	virtual void Traverse(const SymbolVisitor& visitor) override {}
	virtual RenderReturn DrawTree(cooking::DisplayList* dlist, const RenderParams& rp, const Sprite* spr = nullptr) const override;
#ifndef S2_DISABLE_FLATTEN
	virtual RenderReturn DrawNode(cooking::DisplayList* dlist, const RenderParams& rp, const Sprite* spr, ft::FTList& ft, int pos) const override { return RENDER_SKIP; }
#endif // S2_DISABLE_FLATTEN

	void SetModel(m3::Model* model);

	void SetAABB(const m3::AABB& aabb) { m_aabb = aabb; }
	const m3::AABB& GetAABB() const { return m_aabb; }

protected:
	virtual sm::rect GetBoundingImpl(const Sprite* spr = nullptr, const Actor* actor = nullptr, bool cache = true) const override;

protected:
	m3::Model* m_model;

	m3::AABB m_aabb;

}; // ModelSymbol

}

#endif // S2_DISABLE_MODEL

#endif // _SPRITE2_MODEL_SYMBOL_H_