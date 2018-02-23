#ifndef _SPRITE2_MODEL_SYMBOL_H_
#define _SPRITE2_MODEL_SYMBOL_H_

#include "sprite2/config.h"

#ifndef S2_DISABLE_MODEL

#include "sprite2/Symbol.h"
#include S2_MAT_HEADER

#include <painting3/AABB.h>
#include <SM_Matrix.h>

namespace n3 { class Model; }

namespace s2
{

class ModelSymbol : public VIRTUAL_INHERITANCE Symbol
{
public:
	ModelSymbol();

	/**
	 *  @interface
	 *    Symbol
	 */
	virtual int Type() const override;
	virtual void Traverse(const SymbolVisitor& visitor) override {}
	virtual pt2::RenderReturn DrawTree(cooking::DisplayList* dlist, const RenderParams& rp, const Sprite* spr = nullptr) const override;
#ifndef S2_DISABLE_FLATTEN
	virtual pt2::RenderReturn DrawNode(cooking::DisplayList* dlist, const RenderParams& rp, const Sprite* spr, ft::FTList& ft, int pos) const override { return pt2::RENDER_SKIP; }
#endif // S2_DISABLE_FLATTEN

	void SetModel(const std::shared_ptr<n3::Model>& model) { m_model = model; }
	const std::shared_ptr<n3::Model>& GetModel() const { return m_model; }

	void SetAABB(const pt3::AABB& aabb) { m_aabb = aabb; }
	const pt3::AABB& GetAABB() const { return m_aabb; }

protected:
	virtual sm::rect GetBoundingImpl(const Sprite* spr = nullptr, const Actor* actor = nullptr, bool cache = true) const override;

protected:
	std::shared_ptr<n3::Model> m_model = nullptr;

	pt3::AABB m_aabb;

}; // ModelSymbol

}

#endif // S2_DISABLE_MODEL

#endif // _SPRITE2_MODEL_SYMBOL_H_