#ifndef _SPRITE2_MODEL_SYMBOL_H_
#define _SPRITE2_MODEL_SYMBOL_H_

#include "s2_config.h"

#ifndef S2_DISABLE_MODEL

#include "S2_Symbol.h"

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
	virtual int Type() const;
	virtual void Traverse(const SymbolVisitor& visitor) {}
	virtual RenderReturn DrawTree(const RenderParams& rp, const Sprite* spr = NULL) const;
	virtual RenderReturn DrawNode(cooking::DisplayList* dlist, const RenderParams& rp, const Sprite* spr, ft::FTList& ft, int pos) const { return RENDER_SKIP; }

	void SetModel(m3::Model* model);

	void SetAABB(const m3::AABB& aabb) { m_aabb = aabb; }
	const m3::AABB& GetAABB() const { return m_aabb; }

protected:
	virtual sm::rect GetBoundingImpl(const Sprite* spr = NULL, const Actor* actor = NULL, bool cache = true) const;

protected:
	m3::Model* m_model;

	m3::AABB m_aabb;

}; // ModelSymbol

}

#endif // S2_DISABLE_MODEL

#endif // _SPRITE2_MODEL_SYMBOL_H_