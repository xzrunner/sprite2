#include "DrawNodeDeferred.h"
#include "RenderScissor.h"	// todo
#include "S2_Sprite.h"
#include "S2_Symbol.h"
#include "sprite2/Utility.h"
#include "SprDefault.h"
#include "RenderFilter.h"

#include <SM_Calc.h>
#include <unirender/UR_RenderContext.h>	// todo
#include <shaderlab/ShaderMgr.h>	// todo
#include <cooking/DisplayList.h>

namespace s2
{

RenderReturn DrawNodeDeferred::Draw(cooking::DisplayList* dlist, 
									const Sprite* spr, 
									const RenderParams& rp)
{
	if (!rp.IsDisableCulling() && CullingTestOutside(spr, rp)) {
		return RENDER_OUTSIDE;
	}
	if (!spr->IsDTexForceCached()) {
		return DrawSprImpl(dlist, spr, rp);
	}
	return RENDER_OK;
}

bool DrawNodeDeferred::CullingTestOutside(const Sprite* spr, 
										  const RenderParams& rp)
{
	if (rp.IsDisableCulling()) {
		return false;
	}

	RenderScissor* rs = RenderScissor::Instance();
	if (rs->IsEmpty() && !rp.IsViewRegionValid()) {
		rp.SetDisableCulling(true);
		return false;
	}

	sm::vec2 r_min, r_max;
	sm::rect r = spr->GetSymbol()->GetBounding(spr, rp.actor);
	r_min.Set(r.xmin, r.ymin);
	r_max.Set(r.xmax, r.ymax);
	S2_MAT mat;
	Utility::PrepareMat(rp.mt, spr, rp.actor, mat);
	r_min = mat * r_min;
	r_max = mat * r_max;

	sm::rect sr(r_min, r_max);

	if (!rs->IsEmpty() && rs->CullingTestOutside(sr)) {
		rp.SetDisableCulling(true);
		return true;
	}
	if (rp.IsViewRegionValid())
	{
		if (!sr.IsValid()) {
			rp.SetDisableCulling(true);
			return false;
		} else if (sm::is_rect_contain_rect(rp.GetViewRegion(), sr)) {
			rp.SetDisableCulling(true);
			return false;
		} else if (sm::is_rect_intersect_rect(rp.GetViewRegion(), sr)) {
			rp.SetDisableCulling(false);
			return false;
		} else {
			rp.SetDisableCulling(true);
			return false;
		}
	}
	else
	{
		rp.SetDisableCulling(true);
		return false;
	}
}

RenderReturn DrawNodeDeferred::DrawSprImpl(cooking::DisplayList* dlist, 
										   const Sprite* spr, 
										   const RenderParams& rp)
{
	const RenderShader& spr_s = spr->GetShader();

	RenderShader rs;
	RenderCamera rc;
	if (rp.IsDisableRenderDraw()) {
		rs = *SprDefault::Instance()->Shader();
		rc = *SprDefault::Instance()->Camera();
	} else if (spr->HaveActor()) {
		rs = spr->GetShader() * rp.shader;
		rc = spr->GetCamera() * rp.camera;
		if (rp.actor) {
			rs = rp.actor->GetShader() * rs;
			rc = rp.actor->GetCamera() * rc;
		}
	} else {
		rs = spr->GetShader() * rp.shader;
		rc = spr->GetCamera() * rp.camera;
	}

	ur::RenderContext* rctx = sl::ShaderMgr::Instance()->GetContext();
	switch (rs.GetFastBlend())
	{
	case FBM_NULL:
		rctx->SetBlend(2, 6);		// BLEND_GL_ONE, BLEND_GL_ONE_MINUS_SRC_ALPHA
		rctx->SetBlendEquation(0);	// BLEND_FUNC_ADD
		break;
	case FBM_ADD:
		rctx->SetBlend(2, 2);		// BLEND_GL_ONE, BLEND_GL_ONE
		rctx->SetBlendEquation(0);	// BLEND_FUNC_ADD
		break;
	case FBM_SUBTRACT:
		rctx->SetBlend(2, 6);		// BLEND_GL_ONE, BLEND_GL_ONE_MINUS_SRC_ALPHA
		rctx->SetBlendEquation(1);	// BLEND_FUNC_SUBTRACT
		break;
	}

	BlendMode blend = BM_NULL;
	if (!rp.IsDisableBlend()) {
		blend = rs.GetBlend();
	}

	FilterMode filter = FM_NULL;
	if (!rp.IsDisableFilter() && rs.GetFilter()) {
		filter = rs.GetFilter()->GetMode();
	}

	RenderReturn ret = RENDER_OK;

	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
	if (blend != BM_NULL) 
	{
		// todo
	} 
	else if (filter != FM_NULL) 
	{
		// todo
	} 
	else 
	{
		if (rp.IsChangeShader()) {
			mgr->SetShader(sl::SPRITE2);
		}

		RenderParams* rp_child = RenderParamsPool::Instance()->Pop();
		*rp_child = rp;
		rp_child->camera = rc;
		ret = DrawSprImplFinal(dlist, spr, *rp_child);
		RenderParamsPool::Instance()->Push(rp_child); 
	}

	return ret;
}

RenderReturn DrawNodeDeferred::DrawSprImplFinal(cooking::DisplayList* dlist, 
												const Sprite* spr, 
												const RenderParams& rp)
{
	// todo downsample
	return spr->GetSymbol()->DrawDeferred(dlist, rp, spr);
}

}