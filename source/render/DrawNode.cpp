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
#include "BoundingBox.h"
#include "DrawIntegrate.h"

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
static void (*DTEX_SYM_INSERT)(uint64_t uid, const sm::rect& bounding, int tex_id, int tex_w, int tex_h);
static const float* (*DTEX_SYM_QUERY)(uint64_t uid, int* tex_id);

static uint64_t (*GET_SYM_UID)(const Symbol*);
static uint64_t (*GET_SPR_UID)(const Sprite*);
static uint64_t (*GET_ACTOR_UID)(const Actor*);

void DrawNode::InitCB(void (*after_spr)(const Sprite*, const RenderParams&))
{
	AFTER_SPR = after_spr;
}

void DrawNode::InitDTexCB(void (*prepare_render_params)(const RenderParams& rp, const Sprite* spr, RenderParams& child),
						  void (*dtex_sym_insert)(uint64_t uid, const sm::rect& bounding, int tex_id, int tex_w, int tex_h),
						  const float* dtex_sym_query(uint64_t uid, int* tex_id))
{
	PREPARE_REDNER_PARAMS = prepare_render_params;
	DTEX_SYM_INSERT = dtex_sym_insert;
	DTEX_SYM_QUERY  = dtex_sym_query;
}

void DrawNode::InitUIDCB(uint64_t (*get_sym_uid)(const Symbol*),
						 uint64_t (*get_spr_uid)(const Sprite*),
						 uint64_t (*get_actor_uid)(const Actor*))
{
	GET_SYM_UID = get_sym_uid;
	GET_SPR_UID = get_spr_uid;
	GET_ACTOR_UID = get_actor_uid;
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

	if (rp.min_edge > 0) {
		if (IsSmall(spr, actor, rp.min_edge)) {
			return false;
		}
	}

	Utility::PrepareColor(rp.color, spr, actor, child.color);
	Utility::PrepareMat(rp.mt, spr, actor, child.mt);

	if (PREPARE_REDNER_PARAMS) {
		PREPARE_REDNER_PARAMS(rp, spr, child);
	}

	return true;
}

RenderReturn DrawNode::Draw(const Sprite* spr, const RenderParams& rp)
{
	if (!rp.IsDisableCulling() && CullingTestOutside(spr, rp)) {
		return RENDER_OUTSIDE;
	}
	if (!spr->IsDTexForceCached()) {
		return DrawSprImpl(spr, rp);
	}

	RenderReturn ret = RENDER_OK;
	if (spr->IsDTexForceCachedDirty()) {
		// todo: not draw to cache at the first time
		// i don't know why, but it works
		if (spr->IsDTexCacheBegin()) {
			ret = DTexCacheSpr(spr, rp);
		} else {
			ret = DrawSprImpl(spr, rp);
			spr->SetDTexCacheBegin(true);
		}
	} else {
		bool visible = rp.actor ? rp.actor->IsVisible() : spr->IsVisible();
		if (visible) {
			ret = DTexQuerySpr(spr, rp);
		}
	}
	return ret;
}

RenderReturn DrawNode::Draw(const Symbol* sym, const RenderParams& rp, 
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
 
 	RenderReturn ret = sym->Draw(*rp_child);

	RenderParamsPool::Instance()->Push(rp_child); 

	return ret;
}

RenderReturn DrawNode::Draw(const Symbol* sym, const RenderParams& rp, const S2_MAT& _mt)
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

	RenderReturn ret = sym->Draw(*rp_child);

	RenderParamsPool::Instance()->Push(rp_child); 

	return ret;
}

RenderReturn DrawNode::DrawAABB(const Sprite* spr, const RenderParams& rp, const Color& col)
{
	RenderParams* rp_child = RenderParamsPool::Instance()->Pop();
	*rp_child = rp;
	if (!DrawNode::Prepare(rp, spr, *rp_child)) {
		RenderParamsPool::Instance()->Push(rp_child);
		return RENDER_INVISIBLE;
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

	return RENDER_OK;
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
		if (!sr.IsValid()) {
			rp.SetDisableCulling(true);
			return false;
		} else if (sm::is_rect_contain_rect(rp.view_region, sr)) {
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

RenderReturn DrawNode::DrawSprToRT(const Sprite* spr, const RenderParams& rp, RenderTarget* rt)
{
	rt->Bind();

	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
	mgr->GetContext()->Clear(0);

	RenderParams* rp_child = RenderParamsPool::Instance()->Pop();
	*rp_child = rp;

	if (rp.actor) {
		rp_child->mt = rp.actor->GetLocalMat().Inverted() * spr->GetLocalMat().Inverted();
	} else {
		rp_child->mt = spr->GetLocalMat().Inverted();
	}
	RenderReturn ret = DrawSprImpl(spr, *rp_child);

	sl::ShaderMgr::Instance()->GetShader()->Commit();

	rt->Unbind();

	RenderParamsPool::Instance()->Push(rp_child); 

	return ret;
}

RenderReturn DrawNode::DrawSprFromRT(const Sprite* spr, const RenderParams& rp, const float* texcoords, int tex_id)
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

	return RENDER_OK;
}

RenderReturn DrawNode::DrawSymToRT(const Symbol* sym, RenderTarget* rt)
{
	rt->Bind();

	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
	mgr->GetContext()->Clear(0);

	RenderParams* rp = RenderParamsPool::Instance()->Pop();
	rp->Reset();

	RenderReturn ret = Draw(sym, *rp, S2_MAT());

	sl::ShaderMgr::Instance()->GetShader()->Commit();

	rt->Unbind();

	RenderParamsPool::Instance()->Push(rp); 

	return ret;
}

RenderReturn DrawNode::DTexCacheSym(const Symbol* sym)
{
	RenderTargetMgr* RT = RenderTargetMgr::Instance();
	RenderTarget* rt = RT->Fetch();
	if (!rt) {
		return RENDER_NO_RT;
	}

	RenderReturn ret = RENDER_OK;

	sl::ShaderMgr::Instance()->FlushShader();

	RenderScissor::Instance()->Disable();
	RenderCtxStack::Instance()->Push(RenderContext(RT->WIDTH, RT->HEIGHT, RT->WIDTH, RT->HEIGHT));

	RenderReturn r = DrawSymToRT(sym, rt);
	if (r != RENDER_OK) {
		ret = r;
	}

	RenderCtxStack::Instance()->Pop();
	RenderScissor::Instance()->Enable();

	DTEX_SYM_INSERT(GET_SYM_UID(sym), sym->GetBounding(), rt->GetTexID(), rt->Width(), rt->Height());

	RT->Return(rt);

	return ret;
}

const float* DrawNode::DTexQuerySym(const Symbol* sym, int& tex_id)
{
	return DTEX_SYM_QUERY(GET_SYM_UID(sym), &tex_id);
}

RenderReturn DrawNode::DTexCacheSpr(const Sprite* spr, const RenderParams& rp)
{
	RenderTargetMgr* RT = RenderTargetMgr::Instance();
	RenderTarget* rt = RT->Fetch();
	if (!rt) {
		return RENDER_NO_RT;
	}

	RenderReturn ret = RENDER_OK;

	sl::ShaderMgr::Instance()->FlushShader();

	RenderScissor::Instance()->Disable();
	RenderCtxStack::Instance()->Push(RenderContext(RT->WIDTH, RT->HEIGHT, RT->WIDTH, RT->HEIGHT));

	ret |= DrawSprToRT(spr, rp, rt);
	bool loading_finished = (ret & RENDER_ON_LOADING) == 0;

	RenderCtxStack::Instance()->Pop();
	RenderScissor::Instance()->Enable();

	if (loading_finished) 
	{
		uint64_t uid = rp.actor ? GET_ACTOR_UID(rp.actor) : GET_SPR_UID(spr);
		const sm::rect& bounding = spr->GetSymbol()->GetBounding(spr, rp.actor);		
		DTEX_SYM_INSERT(uid, bounding, rt->GetTexID(), rt->Width(), rt->Height());
		ret |= RENDER_UNKNOWN;
	}

	RT->Return(rt);

	if (loading_finished) {
		spr->SetDTexForceCachedDirty(false);
	}

	return ret;
}

RenderReturn DrawNode::DTexQuerySpr(const Sprite* spr, const RenderParams& rp)
{
	int tex_id;
	uint64_t uid = rp.actor ? GET_ACTOR_UID(rp.actor) : GET_SPR_UID(spr);
	const float* texcoords = DTEX_SYM_QUERY(uid, &tex_id);
	RenderReturn ret = RENDER_OK;
	if (!texcoords) {
		ret = DrawSprImpl(spr, rp);
		spr->SetDTexForceCachedDirty(true);
	} else {
		ret = DrawSprFromRT(spr, rp, texcoords, tex_id);
	}
	return ret;
}

RenderReturn DrawNode::DrawSprImpl(const Sprite* spr, const RenderParams& rp)
{
	if (spr->IsIntegrate()) {
		return DrawIntegrate::Draw(spr, rp);
	}

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
		// 		const Camera* cam = CameraMgr::Instance()->GetCamera();
		// 		if (cam->Type() == "ortho") {
		ret = DrawBlend::Draw(spr, rp);
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
			ret = DrawGaussianBlur::Draw(spr, *rp_child, itrs);
		} 
		else if (filter == FM_OUTER_GLOW) 
		{
			int itrs = static_cast<const RFOuterGlow*>(rf)->GetIterations();
			ret = DrawOuterGlow::Draw(spr, *rp_child, itrs);
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
			ret = DrawSprImplFinal(spr, *rp_child);
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
		ret = DrawSprImplFinal(spr, *rp_child);
		RenderParamsPool::Instance()->Push(rp_child); 
	}

	return ret;
}

RenderReturn DrawNode::DrawSprImplFinal(const Sprite* spr, const RenderParams& rp)
{
// 	// for debug
// 	if (spr->GetSymbol()->GetID() == 1079524) {
// 		DrawAABB(spr, rp, Color(255, 0, 0));
// 	}

	RenderReturn ret = RENDER_OK;
	float ds = spr->GetShader().GetDownsample();
	if (fabs(ds - 1) > FLT_EPSILON) {
		DrawDownsample::Draw(spr, rp, ds);
	} else {
		ret = spr->GetSymbol()->Draw(rp, spr);
	}
	if (AFTER_SPR) {
		AFTER_SPR(spr, rp);
	}

	return ret;
}

bool DrawNode::IsSmall(const Sprite* spr, const Actor* actor, int min_edge)
{
	if (actor) {
		const sm::rect& rect = actor->GetAABB().GetRect();
		if (rect.xmax - rect.xmin < min_edge &&
			rect.ymax - rect.ymin < min_edge) {
			return true;
		}
	}
	if (spr) {
		sm::vec2 sz = spr->GetBounding(actor)->GetSize().Size();
		if (sz.x < min_edge && sz.y < min_edge) {
			return true;
		}
	}
	return false;
}

}