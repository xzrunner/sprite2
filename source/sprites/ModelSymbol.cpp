#include "sprite2/ModelSymbol.h"

#ifndef S2_DISABLE_MODEL

#include "sprite2/SymType.h"
#include "sprite2/ModelSprite.h"
#include "sprite2/DrawNode.h"

#include <node3/RenderParams.h>
#include <node3/PrimitiveDraw.h>

namespace s2
{

ModelSymbol::ModelSymbol()
{
}

int ModelSymbol::Type() const
{
	return SYM_MODEL;
}

RenderReturn ModelSymbol::DrawTree(cooking::DisplayList* dlist, const RenderParams& rp, const Sprite* spr) const
{
 	if (!spr || !m_model) {
 		return RENDER_NO_DATA;
 	}

	RenderParamsProxy rp_proxy;
	RenderParams* rp_child = rp_proxy.obj;
	memcpy(rp_child, &rp, sizeof(rp));

	if (!DrawNode::Prepare(rp, spr, *rp_child)) {
		return RENDER_INVISIBLE;
	}

	auto spr_model = dynamic_cast<const ModelSprite*>(spr);

	auto& pos3 = spr_model->GetPos3();
	const float* mt6 = rp_child->mt.x;
	sm::mat4 mt2d;
	mt2d.x[0]  = mt6[0];
	mt2d.x[1]  = mt6[1];
	mt2d.x[4]  = mt6[2];
	mt2d.x[5]  = mt6[3];
	mt2d.x[12] = mt6[4] / 128;
	mt2d.x[13] = mt6[5] / 128;
	sm::mat4 mt_rot = sm::mat4(spr_model->GetOri3());
 	sm::mat4 mt = mt2d * mt_rot * sm::mat4::Translated(pos3.x, pos3.y, pos3.z);
	m_model->Draw(n3::RenderParams(mt, mt_rot));

	n3::PrimitiveDraw::SetColor(0xff00ff00);
	n3::PrimitiveDraw::Cube(mt, m_aabb);

	return RENDER_OK;
}

sm::rect ModelSymbol::GetBoundingImpl(const Sprite* spr, const Actor* actor, bool cache) const
{
	// empty
	return sm::rect();
}

}

#endif // S2_DISABLE_MODEL