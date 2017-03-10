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

#include <SM_Calc.h>
#include <unirender/UR_RenderContext.h>
#include <shaderlab/ShaderMgr.h>
#include <shaderlab/FilterShader.h>
#include <shaderlab/EdgeDetectProg.h>
#include <shaderlab/Sprite2Shader.h>

namespace s2
{

static void (*AFTER_SPR)(const Sprite*, const RenderParams&);

static void (*PREPARE_REDNER_PARAMS)(const RenderParams& parent, const Sprite* spr, RenderParams& child);
static void (*C2_INSERT_SPR)(const Sprite*, int tex_id, int tex_w, int tex_h);
static const float* (*C2_QUERY_SPR)(const s2::Sprite* spr, int* tex_id);

void DrawNode::InitCB(void (*after_spr)(const Sprite*, const RenderParams&))
{
	AFTER_SPR = after_spr;
}

void DrawNode::InitDTexCB(void (*prepare_render_params)(const RenderParams& parent, const Sprite* spr, RenderParams& child),
						  void (*c2_insert_spr)(const s2::Sprite*, int tex_id, int tex_w, int tex_h),
						  const float* c2_query_spr(const s2::Sprite* spr, int* tex_id))
{
	PREPARE_REDNER_PARAMS = prepare_render_params;
	C2_INSERT_SPR = c2_insert_spr;
	C2_QUERY_SPR = c2_query_spr;
}

bool DrawNode::Prepare(const RenderParams& parent, const Sprite* spr, RenderParams& child)
{
	if (!spr) {
		child = parent;
		return true;
	}
	if (!spr->IsVisible()) {
		return false;
	}

	child = parent;
	child.path.Push(spr->GetID());
	const Actor* actor = spr->QueryActor(child.path);
	if (actor && !actor->IsVisible()) {
		return false;
	}

	child.mt = spr->GetLocalMat() * parent.mt;
	child.color = spr->GetColor() * parent.color;
	if (actor) {
		child.mt = actor->GetLocalMat() * child.mt;
		child.color = actor->GetColor() * child.color;
	}

	if (PREPARE_REDNER_PARAMS) {
		PREPARE_REDNER_PARAMS(parent, spr, child);
	}

	return true;
}

S2_MAT DrawNode::PrepareMat(const RenderParams& parent, const Sprite* spr)
{
	S2_MAT mat = spr->GetLocalMat() * parent.mt;
	if (!spr->HaveActor()) {
		return mat;
	}

	SprTreePath path = parent.path;
	path.Push(spr->GetID());
	const Actor* actor = spr->QueryActor(path);
	if (actor) {
		mat = actor->GetLocalMat() * mat;
	}
	return mat;
}

void DrawNode::Draw(const Sprite* spr, const RenderParams& rp, bool scissor)
{
	if (scissor && IsOutsideView(spr, rp)) {
		return;
	}

	if (spr->IsDTexForceCached())
	{
		if (spr->IsDTexForceCachedDirty())
		{
			RenderTargetMgr* RT = RenderTargetMgr::Instance();
			RenderTarget* rt = RT->Fetch();
			if (!rt) {
				return;
			}

			sl::ShaderMgr::Instance()->FlushShader();

			RenderScissor::Instance()->Close();
			RenderCtxStack::Instance()->Push(RenderContext(RT->WIDTH, RT->HEIGHT, RT->WIDTH, RT->HEIGHT));

			DTexDrawSprToRT(spr, rp, rt);

			RenderCtxStack::Instance()->Pop();
			RenderScissor::Instance()->Open();

			C2_INSERT_SPR(spr, rt->GetTexID(), rt->Width(), rt->Height());

			RT->Return(rt);

			spr->SetDTexForceCachedDirty(false);
		}
		else
		{
			int tex_id;
			const float* texcoords = C2_QUERY_SPR(spr, &tex_id);
			if (!texcoords) {
				DrawSprImpl(spr, rp);
				spr->SetDTexForceCachedDirty(true);
			} else {
				DTexDrawSprFromRT(spr, rp, texcoords, tex_id);
			}
		}
	}
	else
	{
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

 	RenderParams rp_child = rp;
 	rp_child.mt = mt;
 
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
 		if (rp_child.IsChangeShader()) {
 			mgr->SetShader(sl::FILTER);
 			sl::FilterShader* shader = static_cast<sl::FilterShader*>(mgr->GetShader());
 			shader->SetMode(sl::FILTER_MODE(filter));
 		}
 	} else {
 		if (rp_child.IsChangeShader()) {
 			mgr->SetShader(sl::SPRITE2);
 		}
 	}
 
 	sym->Draw(rp_child);
}

void DrawNode::Draw(const Symbol* sym, const RenderParams& rp, const S2_MAT& _mt)
{
	S2_MAT mt = _mt * rp.mt;

	RenderParams rp_child = rp;
	rp_child.mt = mt;

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
		if (rp_child.IsChangeShader()) {
			mgr->SetShader(sl::FILTER);
			sl::FilterShader* shader = static_cast<sl::FilterShader*>(mgr->GetShader());
			shader->SetMode(sl::FILTER_MODE(filter));
		}
	} else {
		if (rp_child.IsChangeShader()) {
			mgr->SetShader(sl::SPRITE2);
		}
	}

	sym->Draw(rp_child);
}

bool DrawNode::IsOutsideView(const Sprite* spr, const RenderParams& rp)
{	
	if (!rp.view_region.IsValid()) {
		return false;
	}

	sm::rect r = spr->GetSymbol()->GetBounding(spr);
	S2_MAT mat = PrepareMat(rp, spr);
	sm::vec2 r_min = mat * sm::vec2(r.xmin, r.ymin);
	sm::vec2 r_max = mat * sm::vec2(r.xmax, r.ymax);
	return !is_rect_intersect_rect(rp.view_region, sm::rect(r_min, r_max));
}

void DrawNode::DTexDrawSprToRT(const Sprite* spr, const RenderParams& rp, RenderTarget* rt)
{
	rt->Bind();

	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
	mgr->GetContext()->Clear(0);

	RenderParams rp_child = rp;
	S2_MAT mt = DrawNode::PrepareMat(rp, spr);
	rp_child.mt = mt.Inverted();
	DrawSprImpl(spr, rp_child);

	sl::ShaderMgr::Instance()->GetShader()->Commit();

	rt->Unbind();
}

void DrawNode::DTexDrawSprFromRT(const Sprite* spr, const RenderParams& rp, const float* texcoords, int tex_id)
{
	sm::vec2 vertices[4];
	sm::rect r = spr->GetSymbol()->GetBounding(spr);
	vertices[0] = sm::vec2(r.xmin, r.ymin);
	vertices[1] = sm::vec2(r.xmax, r.ymin);
	vertices[2] = sm::vec2(r.xmax, r.ymax);
	vertices[3] = sm::vec2(r.xmin, r.ymax);
	S2_MAT mt = DrawNode::PrepareMat(rp, spr);
	for (int i = 0; i < 4; ++i) {
		vertices[i] = mt * vertices[i];
	}

	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
	mgr->SetShader(sl::SPRITE2);
	sl::Sprite2Shader* shader = static_cast<sl::Sprite2Shader*>(mgr->GetShader(sl::SPRITE2));
	shader->SetColor(0xffffffff, 0);
	shader->SetColorMap(0x000000ff, 0x0000ff00, 0x00ff0000);
	shader->Draw(&vertices[0].x, texcoords, tex_id);
}

void DrawNode::DrawSprImpl(const Sprite* spr, const RenderParams& rp)
{
	RenderShader rs;
	RenderCamera rc;

	if (rp.IsDisableRenderDraw()) {
		rs = *SprDefault::Instance()->Shader();
		rc = *SprDefault::Instance()->Camera();
	} else if (spr->HaveActor()) {
		SprTreePath path = rp.path;
		path.Push(spr->GetID());
		const Actor* actor = spr->QueryActor(path);
		if (actor) {
			rs = actor->GetShader() * rp.shader;
			rc = actor->GetCamera() * rp.camera;
		} else {
			rs = spr->GetShader() * rp.shader;
			rc = spr->GetCamera() * rp.camera;
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

		RenderParams t = rp;
		t.shader.SetFilter(rf);
		t.camera = rc;
		if (filter == FM_GAUSSIAN_BLUR) 
		{
			int itrs = static_cast<const RFGaussianBlur*>(rf)->GetIterations();
			DrawGaussianBlur::Draw(spr, t, itrs);
		} 
		else if (filter == FM_OUTER_GLOW) 
		{
			int itrs = static_cast<const RFOuterGlow*>(rf)->GetIterations();
			DrawOuterGlow::Draw(spr, t, itrs);
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
			DrawSprImplFinal(spr, t);
		}
	} 
	else 
	{
		if (rp.IsChangeShader()) {
			mgr->SetShader(sl::SPRITE2);
		}
		RenderParams t = rp;
		t.camera = rc;
		DrawSprImplFinal(spr, t);
	}
}

void DrawNode::DrawSprImplFinal(const Sprite* spr, const RenderParams& rp)
{
	spr->GetSymbol()->Draw(rp, spr);
	if (AFTER_SPR) {
		AFTER_SPR(spr, rp);
	}
}

}