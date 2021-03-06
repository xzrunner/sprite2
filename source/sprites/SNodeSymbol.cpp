#include "sprite2/SNodeSymbol.h"

#ifndef S2_DISABLE_SNODE

#include "sprite2/SNodeSprite.h"
#include "sprite2/SymType.h"
#include "sprite2/DrawNode.h"

#include <painting2/Blackboard.h>
#include <painting2/RenderContext.h>
#include <painting2/WindowContext.h>
#include <painting3/PrimitiveDraw.h>
#include <painting3/Blackboard.h>
#include <painting3/WindowContext.h>
#include <node3/RenderParams.h>
#include <node3/DrawNode.h>

namespace s2
{

SNodeSymbol::SNodeSymbol()
{
}

int SNodeSymbol::Type() const
{
	return SYM_SNODE;
}

pt2::RenderReturn SNodeSymbol::DrawTree(cooking::DisplayList* dlist, const RenderParams& rp, const Sprite* spr) const
{
 	if (!spr || !m_node) {
 		return pt2::RENDER_NO_DATA;
 	}

	RenderParamsProxy rp_proxy;
	RenderParams* rp_child = rp_proxy.obj;
	memcpy(rp_child, &rp, sizeof(rp));

	if (!DrawNode::Prepare(rp, spr, *rp_child)) {
		return pt2::RENDER_INVISIBLE;
	}

	sm::mat4 mt2d = CalcCam2dMat(rp_child->mt);
	sm::mat4 mt3d = CalcCam3dMat();

	auto spr_model = dynamic_cast<const SNodeSprite*>(spr);
	sm::mat4 mt_rot = sm::mat4(spr_model->GetOri3());
	auto& pos3 = spr_model->GetPos3();
	sm::mat4 mt_trans = sm::mat4::Translated(pos3.x, pos3.y, pos3.z);

 	sm::mat4 mt = mt2d * mt3d * mt_rot * mt_trans;
	n3::DrawNode::Draw(m_node, mt);

	//pt3::PrimitiveDraw::SetColor(0xff00ff00);
	//pt3::PrimitiveDraw::Cube(mt, m_aabb);

	return pt2::RENDER_OK;
}

sm::rect SNodeSymbol::GetBoundingImpl(const Sprite* spr, const Actor* actor, bool cache) const
{
	// empty
	return sm::rect();
}

sm::mat4 SNodeSymbol::CalcCam2dMat(const S2_MAT& mt2)
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

	auto& wc = pt2::Blackboard::Instance()->GetWindowContext();
	GD_ASSERT(wc, "null wc");
	auto& translate = wc->GetMVOffset();
	float scale = wc->GetMVScale();
	sm::mat4 mt_scale = sm::mat4::Scaled(scale, scale, 1);
	sm::mat4 mt_translate = sm::mat4::Translated(translate.x * SCALE, translate.y * SCALE, 0);

	return mt16 * mt_translate * mt_scale;
}

sm::mat4 SNodeSymbol::CalcCam3dMat()
{
	auto wc = pt3::Blackboard::Instance()->GetWindowContext();
	return wc ? wc->GetModelViewMat() : sm::mat4();
}

}

#endif // S2_DISABLE_SNODE