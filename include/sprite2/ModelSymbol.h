#ifndef _SPRITE2_MODEL_SYMBOL_H_
#define _SPRITE2_MODEL_SYMBOL_H_

#include "sprite2/config.h"

#ifndef S2_DISABLE_MODEL

#include "sprite2/Symbol.h"
#include S2_MAT_HEADER

#include <node0/typedef.h>
#include <SM_Matrix.h>

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
	virtual RenderReturn DrawTree(cooking::DisplayList* dlist, const RenderParams& rp, const Sprite* spr = nullptr) const override;
#ifndef S2_DISABLE_FLATTEN
	virtual RenderReturn DrawNode(cooking::DisplayList* dlist, const RenderParams& rp, const Sprite* spr, ft::FTList& ft, int pos) const override { return RENDER_SKIP; }
#endif // S2_DISABLE_FLATTEN

	void SetNode(const n0::SceneNodePtr& node) { m_node = node; }
	const n0::SceneNodePtr& GetNode() const { return m_node; }

protected:
	virtual sm::rect GetBoundingImpl(const Sprite* spr = nullptr, const Actor* actor = nullptr, bool cache = true) const override;

private:
	static sm::mat4 CalcCam2dMat(const S2_MAT& mt2);
	static sm::mat4 CalcCam3dMat();

protected:
	n0::SceneNodePtr m_node = nullptr;

}; // ModelSymbol

}

#endif // S2_DISABLE_MODEL

#endif // _SPRITE2_MODEL_SYMBOL_H_