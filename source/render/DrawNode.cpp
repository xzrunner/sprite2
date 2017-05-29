#include "DrawNode.h"
#include "S2_Symbol.h"
#include "S2_Sprite.h"
#include "S2_Actor.h"
#include "RenderFilter.h"
#include "DrawBlend.h"
#include "RFGaussianBlur.h"
#include "RFEdgeDetection.h"
#include "DrawGaussianBlur.h"
#include "DrawOuterGlow.h"
#include "RFOuterGlow.h"
#include "SprDefault.h"
#include "RenderCtxStack.h"
#include "S2_RenderTargetMgr.h"
#include "RenderScissor.h"
#include "S2_RenderTarget.h"
#include "S2_RVG.h"
#include "DrawDownsample.h"
#include "sprite2/Utility.h"

#include <SM_Calc.h>
#include <unirender/UR_RenderContext.h>
#include <shaderlab/ShaderMgr.h>
#include <shaderlab/FilterShader.h>
#include <shaderlab/EdgeDetectProg.h>
#include <shaderlab/Sprite2Shader.h>

namespace s2
{

static void (*AFTER_SPR)(const Sprite*, const RenderParams&);

static void (*PREPARE_REDNER_PARAMS)(const RenderParams& rp, const Sprite* spr, RenderParams& child);
static void (*C2_INSERT_SPR)(const Sprite*, int tex_id, int tex_w, int tex_h);
static const float* (*C2_QUERY_SPR)(const Sprite*, int* tex_id);
static void (*C2_INSERT_SYM)(const Symbol*, int tex_id, int tex_w, int tex_h);
static const float* (*C2_QUERY_SYM)(const Symbol*, int* tex_id);

void DrawNode::InitCB(void (*after_spr)(const Sprite*, const RenderParams&))
{
	AFTER_SPR = after_spr;
}

void DrawNode::InitDTexCB(void (*prepare_render_params)(const RenderParams& rp, const Sprite* spr, RenderParams& child),
						  void (*c2_insert_spr)(const Sprite*, int tex_id, int tex_w, int tex_h),
						  const float* c2_query_spr(const Sprite*, int* tex_id),
						  void (*c2_insert_sym)(const Symbol*, int tex_id, int tex_w, int tex_h),
						  const float* c2_query_sym(const Symbol*, int* tex_id))
{
	PREPARE_REDNER_PARAMS = prepare_render_params;
	C2_INSERT_SPR = c2_insert_spr;
	C2_QUERY_SPR  = c2_query_spr;
	C2_INSERT_SYM = c2_insert_sym;
	C2_QUERY_SYM  = c2_query_sym;
}

bool DrawNode::Prepare(const RenderParams& rp, const Sprite* spr, RenderParams& child)
{
	if (!spr) {
		child = rp;
		return true;
	}
	if (!spr->IsVisible()) {
		return false;
	}

	const Actor* actor = rp.actor;
	child.actor = actor;
	if (actor && !actor->IsVisible()) {
		child = rp;
		return false;
	}

	Utility::PrepareColor(rp.color, spr, actor, child.color);
	Utility::PrepareMat(rp.mt, spr, actor, child.mt);

	if (PREPARE_REDNER_PARAMS) {
		PREPARE_REDNER_PARAMS(rp, spr, child);
	}

	return true;
}

void DrawNode::Draw(const Sprite* spr, const RenderParams& rp)
{
	if (CullingTestOutside(spr, rp)) {
		return;
	} 

	if (spr->IsDTexForceCached()) {
		if (spr->IsDTexForceCachedDirty()) {
			DTexCacheSpr(spr, rp);
		} else {
			DTexQuerySpr(spr, rp);
		}
	} else {
		DrawSprImpl(spr, rp);
	}
}

void DrawNode::Draw(const Symbol* sym, const RenderParams& rp, 
					const sm::vec2& pos, float angle, 
					const sm::vec2& scale, const sm::vec2& shear)
{
	S2_MAT mt;
	mt.SetTransformation(pos.x, pos.y, angle, scale.x, scale.y, 0, 0, shear.x, shear.y);
	mt = mt * rp.mt;

	RenderParams* rp_child = RenderParamsPool::Instance()->Pop();
	*rp_child = rp;

 	rp_child->mt = mt;
 
	BlendMode blend = BM_NULL;
	if (!rp.IsDisableBlend()) {
		blend = rp.shader.GetBlend();
	}

	FilterMode filter = FM_NULL;
	if (!rp.IsDisableFilter() && rp.shader.GetFilter()) {
		filter = rp.shader.GetFilter()->GetMode();
	}

 	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
 	if (blend != BM_NULL) {
 		;
 	} else if (filter != FM_NULL) {
 		if (rp_child->IsChangeShader()) {
 			mgr->SetShader(sl::FILTER);
 			sl::FilterShader* shader = static_cast<sl::FilterShader*>(mgr->GetShader());
 			shader->SetMode(sl::FILTER_MODE(filter));
 		}
 	} else {
 		if (rp_child->IsChangeShader()) {
 			mgr->SetShader(sl::SPRITE2);
 		}
 	}
 
 	sym->Draw(*rp_child);

	RenderParamsPool::Instance()->Push(rp_child); 
}

void DrawNode::Draw(const Symbol* sym, const RenderParams& rp, const S2_MAT& _mt)
{
	S2_MAT mt = _mt * rp.mt;

	RenderParams* rp_child = RenderParamsPool::Instance()->Pop();
	*rp_child = rp;

	rp_child->mt = mt;

	BlendMode blend = BM_NULL;
	if (!rp.IsDisableBlend()) {
		blend = rp.shader.GetBlend();
	}

	FilterMode filter = FM_NULL;
	if (!rp.IsDisableFilter() && rp.shader.GetFilter()) {
		filter = rp.shader.GetFilter()->GetMode();
	}

	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
	if (blend != BM_NULL) {
		;
	} else if (filter != FM_NULL) {
		if (rp_child->IsChangeShader()) {
			mgr->SetShader(sl::FILTER);
			sl::FilterShader* shader = static_cast<sl::FilterShader*>(mgr->GetShader());
			shader->SetMode(sl::FILTER_MODE(filter));
		}
	} else {
		if (rp_child->IsChangeShader()) {
			mgr->SetShader(sl::SPRITE2);
		}
	}

	sym->Draw(*rp_child);

	RenderParamsPool::Instance()->Push(rp_child); 
}

void DrawNode::DrawAABB(const Sprite* spr, const RenderParams& rp, const Color& col)
{
	RenderParams* rp_child = RenderParamsPool::Instance()->Pop();
	*rp_child = rp;
	if (!DrawNode::Prepare(rp, spr, *rp_child)) {
		RenderParamsPool::Instance()->Push(rp_child);
		return;
	}

	sl::ShaderType prev_shader = sl::ShaderMgr::Instance()->GetShaderType();

	std::vector<sm::vec2> vertices(4);
	sm::rect rect = spr->GetSymbol()->GetBounding(spr, rp.actor);
	vertices[0] = sm::vec2(rect.xmin, rect.ymin);
	vertices[1] = sm::vec2(rect.xmin, rect.ymax);
	vertices[2] = sm::vec2(rect.xmax, rect.ymax);
	vertices[3] = sm::vec2(rect.xmax, rect.ymin);
	for (int i = 0; i < 4; ++i) {
		vertices[i] = rp_child->mt * vertices[i];
	}

	RVG::SetColor(col);
	RVG::Polyline(vertices, true);

	sl::ShaderMgr::Instance()->SetShader(prev_shader);

	RenderParamsPool::Instance()->Push(rp_child); 
}

bool DrawNode::CullingTestOutside(const Sprite* spr, const RenderParams& rp)
{
	if (rp.IsDisableCulling()) {
		return false;
	}

	RenderScissor* rs = RenderScissor::Instance();
	if (rs->IsEmpty() && !rp.view_region.IsValid()) {
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
	if (rp.view_region.IsValid())
	{
		if (sm::is_rect_contain_rect(rp.view_region, sr)) {
			rp.SetDisableCulling(true);
			return false;
		} else if (sm::is_rect_intersect_rect(rp.view_region, sr)) {
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

void DrawNode::DrawSprToRT(const Sprite* spr, const RenderParams& rp, RenderTarget* rt)
{
	rt->Bind();

	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
	mgr->GetContext()->Clear(0);

	RenderParams* rp_child = RenderParamsPool::Instance()->Pop();
	*rp_child = rp;

	rp_child->mt = spr->GetLocalInvMat();
	DrawSprImpl(spr, *rp_child);

	sl::ShaderMgr::Instance()->GetShader()->Commit();

	rt->Unbind();

	RenderParamsPool::Instance()->Push(rp_child); 
}

void DrawNode::DrawSprFromRT(const Sprite* spr, const RenderParams& rp, const float* texcoords, int tex_id)
{
	sm::vec2 vertices[4];
	sm::rect r = spr->GetSymbol()->GetBounding(spr, rp.actor);
	vertices[0] = sm::vec2(r.xmin, r.ymin);
	vertices[1] = sm::vec2(r.xmax, r.ymin);
	vertices[2] = sm::vec2(r.xmax, r.ymax);
	vertices[3] = sm::vec2(r.xmin, r.ymax);
	S2_MAT mt;
	Utility::PrepareMat(rp.mt, spr, rp.actor, mt);
	for (int i = 0; i < 4; ++i) {
		vertices[i] = mt * vertices[i];
	}

	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
	mgr->SetShader(sl::SPRITE2);
	sl::Sprite2Shader* shader = static_cast<sl::Sprite2Shader*>(mgr->GetShader(sl::SPRITE2));
	shader->SetColor(0xffffffff, 0);
	shader->SetColorMap(0x000000ff, 0x0000ff00, 0x00ff0000);
	shader->DrawQuad(&vertices[0].x, texcoords, tex_id);
}

void DrawNode::DrawSymToRT(const Symbol* sym, RenderTarget* rt)
{
	rt->Bind();

	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
	mgr->GetContext()->Clear(0);

	RenderParams* rp = RenderParamsPool::Instance()->Pop();
	rp->Reset();

	Draw(sym, *rp, S2_MAT());

	sl::ShaderMgr::Instance()->GetShader()->Commit();

	rt->Unbind();

	RenderParamsPool::Instance()->Push(rp); 
}

void DrawNode::DTexCacheSym(const Symbol* sym)
{
	RenderTargetMgr* RT = RenderTargetMgr::Instance();
	RenderTarget* rt = RT->Fetch();
	if (!rt) {
		return;
	}

	sl::ShaderMgr::Instance()->FlushShader();

	RenderScissor::Instance()->Disable();
	RenderCtxStack::Instance()->Push(RenderContext(RT->WIDTH, RT->HEIGHT, RT->WIDTH, RT->HEIGHT));

	DrawSymToRT(sym, rt);

	RenderCtxStack::Instance()->Pop();
	RenderScissor::Instance()->Enable();

	C2_INSERT_SYM(sym, rt->GetTexID(), rt->Width(), rt->Height());

	RT->Return(rt);
}

const float* DrawNode::DTexQuerySym(const Symbol* sym, int& tex_id)
{
	return C2_QUERY_SYM(sym, &tex_id);
}

void DrawNode::DTexCacheSpr(const Sprite* spr, const RenderParams& rp)
{
	RenderTargetMgr* RT = RenderTargetMgr::Instance();
	RenderTarget* rt = RT->Fetch();
	if (!rt) {
		return;
	}

	sl::ShaderMgr::Instance()->FlushShader();

	RenderScissor::Instance()->Disable();
	RenderCtxStack::Instance()->Push(RenderContext(RT->WIDTH, RT->HEIGHT, RT->WIDTH, RT->HEIGHT));

	DrawSprToRT(spr, rp, rt);

	RenderCtxStack::Instance()->Pop();
	RenderScissor::Instance()->Enable();

	C2_INSERT_SPR(spr, rt->GetTexID(), rt->Width(), rt->Height());

	RT->Return(rt);

	spr->SetDTexForceCachedDirty(false);
}

void DrawNode::DTexQuerySpr(const Sprite* spr, const RenderParams& rp)
{
	int tex_id;
	const float* texcoords = C2_QUERY_SPR(spr, &tex_id);
	if (!texcoords) {
		DrawSprImpl(spr, rp);
		spr->SetDTexForceCachedDirty(true);
	} else {
		DrawSprFromRT(spr, rp, texcoords, tex_id);
	}
}

void DrawNode::DrawSprImpl(const Sprite* spr, const RenderParams& rp)
{
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

	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
	if (blend != BM_NULL) 
	{
		// 		const Camera* cam = CameraMgr::Instance()->GetCamera();
		// 		if (cam->Type() == "ortho") {
		DrawBlend::Draw(spr, rp.mt);
		//		}
	} 
	else if (filter != FM_NULL) 
	{
		const RenderFilter* rf = rs.GetFilter();

		RenderParams* rp_child = RenderParamsPool::Instance()->Pop();
		*rp_child = rp;

		rp_child->shader.SetFilter(rf);
		rp_child->camera = rc;
		if (filter == FM_GAUSSIAN_BLUR) 
		{
			int itrs = static_cast<const RFGaussianBlur*>(rf)->GetIterations();
			DrawGaussianBlur::Draw(spr, *rp_child, itrs);
		} 
		else if (filter == FM_OUTER_GLOW) 
		{
			int itrs = static_cast<const RFOuterGlow*>(rf)->GetIterations();
			DrawOuterGlow::Draw(spr, *rp_child, itrs);
		} 
		else 
		{
			if (rp.IsChangeShader()) {
				mgr->SetShader(sl::FILTER);
			}
			sl::FilterShader* shader = static_cast<sl::FilterShader*>(mgr->GetShader(sl::FILTER));
			shader->SetMode(sl::FILTER_MODE(filter));
			switch (filter)
			{
			case FM_EDGE_DETECTION:
				{
					const RFEdgeDetection* ed = static_cast<const RFEdgeDetection*>(rf);
					sl::EdgeDetectProg* prog = static_cast<sl::EdgeDetectProg*>(shader->GetProgram(sl::FM_EDGE_DETECTION));
					prog->SetBlend(ed->GetBlend());
				}
				break;
			}
			DrawSprImplFinal(spr, *rp_child);
		}

		RenderParamsPool::Instance()->Push(rp_child); 
	} 
	else 
	{
		if (rp.IsChangeShader()) {
			mgr->SetShader(sl::SPRITE2);
		}

		RenderParams* rp_child = RenderParamsPool::Instance()->Pop();
		*rp_child = rp;
		rp_child->camera = rc;
		DrawSprImplFinal(spr, *rp_child);
		RenderParamsPool::Instance()->Push(rp_child); 
	}
}

void DrawNode::DrawSprImplFinal(const Sprite* spr, const RenderParams& rp)
{
// 	// for debug
// 	if (spr->GetSymbol()->GetID() == 1079524) {
// 		DrawAABB(spr, rp, Color(255, 0, 0));
// 	}

	//if (spr->GetDownsample() != 1) {
	//	DrawDownsample::Draw(spr, rp, spr->GetDownsample());
	//} else {
		spr->GetSymbol()->Draw(rp, spr);
	//}
	if (AFTER_SPR) {
		AFTER_SPR(spr, rp);
	}
}

}