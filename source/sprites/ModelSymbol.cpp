#include "sprite2/ModelSymbol.h"

#ifndef S2_DISABLE_MODEL

#include "sprite2/SymType.h"
#include "sprite2/ModelSprite.h"
#include "sprite2/DrawNode.h"
#include "sprite2/RenderCtxStack.h"

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

	sm::mat4 mt2d = CalcMat2D(rp_child->mt);

	auto spr_model = dynamic_cast<const ModelSprite*>(spr);
	sm::mat4 mt_rot = sm::mat4(spr_model->GetOri3());
	auto& pos3 = spr_model->GetPos3();

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

sm::mat4 ModelSymbol::CalcMat2D(const S2_MAT& mt2)
{
	sm::mat4 mt16;
	
	static const float SCALE = 1 / 128.0f;

	const float* mt6 = mt2.x;
	mt16.x[0] = mt6[0];
	mt16.x[1] = mt6[1];
	mt16.x[4] = mt6[2];
	mt16.x[5] = mt6[3];
	mt16.x[12] = mt6[4] * SCALE;
	mt16.x[13] = mt6[5] * SCALE;

	auto ctx = RenderCtxStack::Instance()->Top();
	if (!ctx) {
		return mt16;
	}

	auto& translate = ctx->GetMVOffset();
	float scale = ctx->GetMVScale();
	sm::mat4 cam2d_mt = sm::mat4::Scaled(scale, scale, 1);
	cam2d_mt.Translate(translate.x * scale, translate.y * scale, 0);
	cam2d_mt.x[12] *= SCALE;
	cam2d_mt.x[13] *= SCALE;

	return cam2d_mt * mt16;
}

}

#endif // S2_DISABLE_MODEL