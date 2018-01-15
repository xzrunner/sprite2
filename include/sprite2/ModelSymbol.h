#ifndef _SPRITE2_MODEL_SYMBOL_H_
#define _SPRITE2_MODEL_SYMBOL_H_

#include "sprite2/config.h"

#ifndef S2_DISABLE_MODEL

#include "sprite2/Symbol.h"
#include S2_MAT_HEADER

#include <node3/IModel.h>

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

	void SetModel(const n3::ModelPtr& model) { m_model = model; }
	n3::ModelPtr GetModel() { return m_model; }

protected:
	virtual sm::rect GetBoundingImpl(const Sprite* spr = nullptr, const Actor* actor = nullptr, bool cache = true) const override;

private:
	static sm::mat4 CalcCam2dMat(const S2_MAT& mt2);
	static sm::mat4 CalcCam3dMat();

protected:
	n3::ModelPtr m_model = nullptr;

}; // ModelSymbol

}

#endif // S2_DISABLE_MODEL

#endif // _SPRITE2_MODEL_SYMBOL_H_