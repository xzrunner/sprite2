#include "sprite2/DrawNode.h"
#include "sprite2/Symbol.h"
#include "sprite2/Sprite.h"
#include "sprite2/Actor.h"
#include "sprite2/DrawBlend.h"
#include "sprite2/RFGaussianBlur.h"
#include "sprite2/RFEdgeDetection.h"
#include "sprite2/DrawGaussianBlur.h"
#include "sprite2/DrawOuterGlow.h"
#include "sprite2/RFOuterGlow.h"
#include "sprite2/SprDefault.h"
#include "sprite2/RenderCtxStack.h"
#include "sprite2/RenderTargetMgr.h"
#include "sprite2/RenderScissor.h"
#include "sprite2/RenderTarget.h"
#include "sprite2/RVG.h"
#include "sprite2/DrawDownsample.h"
#include "sprite2/Utility.h"
#include "sprite2/DrawIntegrate.h"

#include <memmgr/Allocator.h>
#include <SM_Calc.h>
#include <unirender/RenderContext.h>
#include <shaderlab/ShaderMgr.h>
#include <shaderlab/FilterShader.h>
#include <shaderlab/EdgeDetectProg.h>
#include <shaderlab/Sprite2Shader.h>
#include <cooking/DisplayList.h>
#ifndef S2_DISABLE_DEFERRED
#include <cooking/Facade.h>
#endif // S2_DISABLE_DEFERRED

namespace s2
{

static void (*AFTER_SPR)(const Sprite*, const RenderParams&);

static void (*PREPARE_REDNER_PARAMS)(const RenderParams& rp, const Sprite* spr, RenderParams& child);
static void (*DTEX_SYM_INSERT)(uint64_t uid, const sm::rect& bounding, int tex_id, int tex_w, int tex_h);
static const float* (*DTEX_SYM_QUERY)(uint64_t uid, int& tex_id, int& block_id);

static uint64_t (*GET_SYM_UID)(const Symbol&);
static uint64_t (*GET_SPR_UID)(const Sprite*);
static uint64_t (*GET_ACTOR_UID)(const Actor*);

void DrawNode::InitCB(void (*after_spr)(const Sprite*, const RenderParams&))
{
	AFTER_SPR = after_spr;
}

void DrawNode::InitDTexCB(void (*prepare_render_params)(const RenderParams& rp, const Sprite* spr, RenderParams& child),
						  void (*dtex_sym_insert)(uint64_t uid, const sm::rect& bounding, int tex_id, int tex_w, int tex_h),
						  const float* dtex_sym_query(uint64_t uid, int& tex_id, int& block_id))
{
	PREPARE_REDNER_PARAMS = prepare_render_params;
	DTEX_SYM_INSERT = dtex_sym_insert;
	DTEX_SYM_QUERY  = dtex_sym_query;
}

void DrawNode::InitUIDCB(uint64_t (*get_sym_uid)(const Symbol&),
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

	auto& actor = rp.actor;
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

	child = rp;

	Utility::PrepareColor(rp.color, spr, actor, child.color);
	Utility::PrepareMat(rp.mt, spr, actor, child.mt);

	if (PREPARE_REDNER_PARAMS) {
		PREPARE_REDNER_PARAMS(rp, spr, child);
	}

	return true;
}

RenderReturn DrawNode::Draw(cooking::DisplayList* dlist, const Sprite* spr, const RenderParams& rp)
{
	if (!rp.IsDisableCulling() && CullingTestOutside(spr, rp)) {
		return RENDER_OUTSIDE;
	}
	if (!spr->IsDTexForceCached()) {
		return DrawSprImpl(dlist, spr, rp);
	}

	// todo: not support deferred draw
	if (dlist) {
		return RENDER_SKIP;
	}

	RenderReturn ret = RENDER_OK;
	if (spr->IsDTexForceCachedDirty()) {
		// todo: not draw to cache at the first time
		// i don't know why, but it works
		if (spr->IsDTexCacheBegin()) {
			ret = DTexCacheSpr(spr, rp);
		} else {
			ret = DrawSprImpl(dlist, spr, rp);
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

RenderReturn DrawNode::Draw(const Symbol& sym, const RenderParams& rp,
							const sm::vec2& pos, float angle, 
							const sm::vec2& scale, const sm::vec2& shear)
{
	S2_MAT mt;
	mt.SetTransformation(pos.x, pos.y, angle, scale.x, scale.y, 0, 0, shear.x, shear.y);
	mt = mt * rp.mt;

	RenderParamsProxy rp_proxy;
	RenderParams* rp_child = rp_proxy.obj;
	memcpy(rp_child, &rp, sizeof(rp));

 	rp_child->mt = mt;
 
	BlendMode blend = BM_NULL;
	if (!rp.IsDisableBlend()) {
		blend = rp.render_blend;
	}

	FilterMode filter = FM_NULL;
#ifdef S2_FILTER_FULL
	if (!rp.IsDisableFilter() && rp.render_filter) {
		filter = rp.render_filter->GetMode();
	}
#else
	filter = rp.render_filter;
#endif // S2_FILTER_FULL

 	if (blend != BM_NULL) {
 		;
 	} else if (filter != FM_NULL) {
 		if (rp_child->IsChangeShader()) 
		{
#ifdef S2_DISABLE_DEFERRED
			sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();

 			mgr->SetShader(sl::FILTER);

 			sl::FilterShader* shader = static_cast<sl::FilterShader*>(mgr->GetShader());
 			shader->SetMode(sl::FILTER_MODE(filter));
#else
			cooking::change_shader(nullptr, sl::FILTER);
			cooking::set_shader_filter_mode(nullptr, filter);
#endif // S2_DISABLE_DEFERRED
 		}
 	} else {
 		if (rp_child->IsChangeShader()) 
		{
#ifdef S2_DISABLE_DEFERRED
			sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
 			mgr->SetShader(sl::SPRITE2);
#else
			cooking::change_shader(nullptr, sl::SPRITE2);
#endif // S2_DISABLE_DEFERRED
 		}
 	}

 	RenderReturn ret = sym.DrawTree(nullptr, *rp_child);

	return ret;
}

RenderReturn DrawNode::Draw(const Symbol& sym, const RenderParams& rp, const S2_MAT& _mt)
{
	S2_MAT mt = _mt * rp.mt;

	RenderParamsProxy rp_proxy;
	RenderParams* rp_child = rp_proxy.obj;
	memcpy(rp_child, &rp, sizeof(rp));

	rp_child->mt = mt;

	BlendMode blend = BM_NULL;
	if (!rp.IsDisableBlend()) {
		blend = rp.render_blend;
	}

	FilterMode filter = FM_NULL;
#ifdef S2_FILTER_FULL
	if (!rp.IsDisableFilter() && rp.render_filter) {
		filter = rp.render_filter->GetMode();
	}
#else
	filter = rp.render_filter;
#endif // S2_FILTER_FULL

	if (blend != BM_NULL) {
		;
	} else if (filter != FM_NULL) {
		if (rp_child->IsChangeShader()) 
		{
#ifdef S2_DISABLE_DEFERRED
			sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
			mgr->SetShader(sl::FILTER);
			sl::FilterShader* shader = static_cast<sl::FilterShader*>(mgr->GetShader());
			shader->SetMode(sl::FILTER_MODE(filter));
#else
			cooking::change_shader(nullptr, sl::FILTER);
			cooking::set_shader_filter_mode(nullptr, filter);
#endif // S2_DISABLE_DEFERRED
		}
	} else {
		if (rp_child->IsChangeShader()) 
		{
#ifdef S2_DISABLE_DEFERRED
			sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
			mgr->SetShader(sl::SPRITE2);
#else
			cooking::change_shader(nullptr, sl::SPRITE2);
#endif // S2_DISABLE_DEFERRED
		}
	}

	RenderReturn ret = sym.DrawTree(nullptr, *rp_child);

	return ret;
}

RenderReturn DrawNode::DrawAABB(cooking::DisplayList* dlist, const Sprite* spr, 
	                            const RenderParams& rp, const Color& col)
{
	RenderParamsProxy rp_proxy;
	RenderParams* rp_child = rp_proxy.obj;
	memcpy(rp_child, &rp, sizeof(rp));

	if (!DrawNode::Prepare(rp, spr, *rp_child)) {
		return RENDER_INVISIBLE;
	}

#ifdef S2_DISABLE_DEFERRED
	sl::ShaderType prev_shader = sl::ShaderMgr::Instance()->GetShaderType();
#else
	assert(dlist);
	int prev_shader = dlist->GetShaderType();
#endif // S2_DISABLE_DEFERRED

	CU_VEC<sm::vec2> vertices;
	vertices.resize(4);
	sm::rect rect = spr->GetSymbol()->GetBounding(spr, rp.actor);
	vertices[0] = sm::vec2(rect.xmin, rect.ymin);
	vertices[1] = sm::vec2(rect.xmin, rect.ymax);
	vertices[2] = sm::vec2(rect.xmax, rect.ymax);
	vertices[3] = sm::vec2(rect.xmax, rect.ymin);
	for (int i = 0; i < 4; ++i) {
		vertices[i] = rp_child->mt * vertices[i];
	}

	RVG::SetColor(col);
	RVG::Polyline(nullptr, vertices, true);

#ifdef S2_DISABLE_DEFERRED
	sl::ShaderMgr::Instance()->SetShader(prev_shader);
#else
	cooking::change_shader(dlist, prev_shader);
#endif // S2_DISABLE_DEFERRED

	return RENDER_OK;
}

bool DrawNode::CullingTestOutside(const Sprite* spr, const RenderParams& rp)
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

RenderReturn DrawNode::DrawSprToRT(const Sprite* spr, const RenderParams& rp, RenderTarget* rt)
{
	rt->Bind();

	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
	mgr->GetContext()->Clear(0);

	RenderParamsProxy rp_proxy;
	RenderParams* rp_child = rp_proxy.obj;
	memcpy(rp_child, &rp, sizeof(rp));

	if (rp.actor) {
		rp_child->mt = rp.actor->GetLocalMat().Inverted() * spr->GetLocalMat().Inverted();
	} else {
		rp_child->mt = spr->GetLocalMat().Inverted();
	}
	RenderReturn ret = DrawSprImpl(nullptr, spr, *rp_child);

	sl::ShaderMgr::Instance()->GetShader()->Commit();

	rt->Unbind();

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

#ifdef S2_DISABLE_DEFERRED
	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();

	mgr->SetShader(sl::SPRITE2);

	sl::Sprite2Shader* shader = static_cast<sl::Sprite2Shader*>(mgr->GetShader(sl::SPRITE2));
	shader->SetColor(0xffffffff, 0);
	shader->SetColorMap(0x000000ff, 0x0000ff00, 0x00ff0000);

	shader->DrawQuad(&vertices[0].x, texcoords, tex_id);
#else
	cooking::change_shader(nullptr, sl::SPRITE2);
	cooking::set_color_sprite(nullptr, 0xffffffff, 0, 0x000000ff, 0x0000ff00, 0x00ff0000);
	cooking::draw_quad_sprite(nullptr, &vertices[0].x, texcoords, tex_id);
#endif // S2_DISABLE_DEFERRED

	return RENDER_OK;
}

RenderReturn DrawNode::DrawSymToRT(const Symbol& sym, RenderTarget* rt)
{
	rt->Bind();

	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
	mgr->GetContext()->Clear(0);

	RenderParamsProxy rp_proxy;
	RenderParams* rp = rp_proxy.obj;
	rp->Reset();

	RenderReturn ret = Draw(sym, *rp, S2_MAT());

	sl::ShaderMgr::Instance()->GetShader()->Commit();

	rt->Unbind();

	return ret;
}

RenderReturn DrawNode::DTexCacheSym(const Symbol& sym)
{
	RenderTargetMgr* RT = RenderTargetMgr::Instance();
	RenderTarget* rt = RT->Fetch();
	if (!rt) {
		return RENDER_NO_RT;
	}

	RenderReturn ret = RENDER_OK;

	sl::ShaderMgr::Instance()->FlushShader();

	RenderScissor::Instance()->Disable();
	RenderCtxStack::Instance()->Push(RenderContext(
		static_cast<float>(RT->WIDTH), static_cast<float>(RT->HEIGHT), RT->WIDTH, RT->HEIGHT));

	RenderReturn r = DrawSymToRT(sym, rt);
	if (r != RENDER_OK) {
		ret = r;
	}

	RenderCtxStack::Instance()->Pop();
	RenderScissor::Instance()->Enable();

	DTEX_SYM_INSERT(GET_SYM_UID(sym), sym.GetBounding(), rt->GetTexID(), rt->Width(), rt->Height());

	RT->Return(rt);

	return ret;
}

const float* DrawNode::DTexQuerySym(const Symbol& sym, int& tex_id, int& block_id)
{
	return DTEX_SYM_QUERY(GET_SYM_UID(sym), tex_id, block_id);
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
	RenderCtxStack::Instance()->Push(RenderContext(
		static_cast<float>(RT->WIDTH), static_cast<float>(RT->HEIGHT), RT->WIDTH, RT->HEIGHT));

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
	int tex_id, block_id;
	uint64_t uid = rp.actor ? GET_ACTOR_UID(rp.actor) : GET_SPR_UID(spr);
	const float* texcoords = DTEX_SYM_QUERY(uid, tex_id, block_id);
	RenderReturn ret = RENDER_OK;
	if (!texcoords) {
		ret = DrawSprImpl(nullptr, spr, rp);
		spr->SetDTexForceCachedDirty(true);
	} else {
		ret = DrawSprFromRT(spr, rp, texcoords, tex_id);
	}
	return ret;
}

RenderReturn DrawNode::DrawSprImpl(cooking::DisplayList* dlist, const Sprite* spr, const RenderParams& rp)
{
	const RenderShader& spr_s = spr->GetShader();

// 	if (!rp.IsDisableIntegrate() && spr->IsIntegrate()) {
// 		return DrawIntegrate().Draw(spr, rp);
// 	}

	RenderShader rs;
	RenderCamera rc;
	if (rp.IsDisableRenderDraw()) {
		rs = *SprDefault::Instance()->Shader();
		rc = *SprDefault::Instance()->Camera();
	} else if (spr->HaveActor()) {
		rs = spr->GetShader().Multiply(rp.render_filter, rp.render_blend, rp.render_fast_blend, rp.render_downsample);
		rc = spr->GetCamera() * rp.camera;
		if (rp.actor) {
			rs = rp.actor->GetShader() * rs;
			rc = rp.actor->GetCamera() * rc;
		}
	} else {
		rs = spr->GetShader().Multiply(rp.render_filter, rp.render_blend, rp.render_fast_blend, rp.render_downsample);
		rc = spr->GetCamera() * rp.camera;
	}

#ifdef S2_DISABLE_DEFERRED
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
#else 
	switch (rs.GetFastBlend())
	{
	case FBM_NULL:
		// BLEND_GL_ONE, BLEND_GL_ONE_MINUS_SRC_ALPHA
		// BLEND_FUNC_ADD
		cooking::set_blend(dlist, 2, 6, 0);
		break;
	case FBM_ADD:
		// BLEND_GL_ONE, BLEND_GL_ONE
		// BLEND_FUNC_ADD
		cooking::set_blend(dlist, 2, 2, 0);
		break;
	case FBM_SUBTRACT:
		// BLEND_GL_ONE, BLEND_GL_ONE_MINUS_SRC_ALPHA
		// BLEND_FUNC_SUBTRACT
		cooking::set_blend(dlist, 2, 6, 1);
		break;
	}
#endif // S2_DISABLE_DEFERRED

	BlendMode blend = BM_NULL;
	if (!rp.IsDisableBlend()) {
		blend = rs.GetBlend();
	}

	FilterMode filter = FM_NULL;
#ifdef S2_FILTER_FULL
	if (!rp.IsDisableFilter() && rs.GetFilter()) {
		filter = rs.GetFilter()->GetMode();
	}
#else
	if (!rp.IsDisableFilter()) {
		filter = rs.GetFilter();
	}
#endif // S2_FILTER_FULL

	RenderReturn ret = RENDER_OK;

#ifdef S2_DISABLE_DEFERRED
	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
#endif // S2_DISABLE_DEFERRED
	if (blend != BM_NULL) 
	{
		// 		const Camera* cam = CameraMgr::Instance()->GetCamera();
		// 		if (cam->Type() == "ortho") {
		ret = DrawBlend().Draw(dlist, spr, rp);
		//		}s
	} 
	else if (filter != FM_NULL) 
	{
#ifdef S2_FILTER_FULL
		auto& rf = rs.GetFilter();

		RenderParamsProxy rp_proxy;
	    RenderParams* rp_child = rp_proxy.obj;
		memcpy(rp_child, &rp, sizeof(rp));

		rp_child->render_filter = rf.get();
		rp_child->camera = rc;
		if (filter == FM_GAUSSIAN_BLUR) 
		{
			int itrs = static_cast<RFGaussianBlur*>(rf.get())->GetIterations();
			ret = DrawGaussianBlur::Draw(dlist, spr, *rp_child, itrs);
		} 
		else if (filter == FM_OUTER_GLOW) 
		{
			int itrs = static_cast<RFOuterGlow*>(rf.get())->GetIterations();
			ret = DrawOuterGlow::Draw(dlist, spr, *rp_child, itrs);
		} 
		else 
		{
			if (rp.IsChangeShader()) {
#ifdef S2_DISABLE_DEFERRED
				mgr->SetShader(sl::FILTER);
#else
				cooking::change_shader(dlist, sl::FILTER);
#endif // S2_DISABLE_DEFERRED
			}
#ifdef S2_DISABLE_DEFERRED
			sl::FilterShader* shader = static_cast<sl::FilterShader*>(mgr->GetShader(sl::FILTER));
			shader->SetMode(sl::FILTER_MODE(filter));
#else
			cooking::set_shader_filter_mode(dlist, filter);
#endif // S2_DISABLE_DEFERRED
			switch (filter)
			{
			case FM_EDGE_DETECTION:
				{
					auto ed = static_cast<RFEdgeDetection*>(rf.get());
					sl::EdgeDetectProg* prog = static_cast<sl::EdgeDetectProg*>(shader->GetProgram(sl::FM_EDGE_DETECTION));
					prog->SetBlend(ed->GetBlend());
				}
				break;
			}
			ret = DrawSprImplFinal(nullptr, spr, *rp_child);
		}
#else
		auto& rf = rs.GetFilter();

		RenderParamsProxy rp_proxy;
		RenderParams* rp_child = rp_proxy.obj;
		memcpy(rp_child, &rp, sizeof(rp));

		rp_child->render_filter = rf;
		rp_child->camera = rc;

		if (rp.IsChangeShader()) {
#ifdef S2_DISABLE_DEFERRED
			mgr->SetShader(sl::FILTER);
#else
			cooking::change_shader(dlist, sl::FILTER);
#endif // S2_DISABLE_DEFERRED
		}
#ifdef S2_DISABLE_DEFERRED
		sl::FilterShader* shader = static_cast<sl::FilterShader*>(mgr->GetShader(sl::FILTER));
		shader->SetMode(sl::FILTER_MODE(filter));
#else
		cooking::change_shader(dlist, sl::FILTER);
		cooking::set_shader_filter_mode(dlist, filter);
#endif // S2_DISABLE_DEFERRED
		ret = DrawSprImplFinal(dlist, spr, *rp_child);
#endif // S2_FILTER_FULL
	} 
	else 
	{
		if (rp.IsChangeShader()) {
#ifdef S2_DISABLE_DEFERRED
			mgr->SetShader(sl::SPRITE2);
#else
			cooking::change_shader(dlist, sl::SPRITE2);
#endif // S2_DISABLE_DEFERRED
		}

		RenderParamsProxy rp_proxy;
		RenderParams* rp_child = rp_proxy.obj;
		memcpy(rp_child, &rp, sizeof(rp));

		rp_child->camera = rc;
		ret = DrawSprImplFinal(dlist, spr, *rp_child);
	}

	return ret;
}

RenderReturn DrawNode::DrawSprImplFinal(cooking::DisplayList* dlist, const Sprite* spr, const RenderParams& rp)
{
// 	// for debug
// 	if (spr->GetSymbol()->GetID() == 1079524) {
// 		DrawAABB(spr, rp, Color(255, 0, 0));
// 	}

	RenderReturn ret = RENDER_OK;
	float ds = spr->GetShader().GetDownsample();
	if (fabs(ds - 1) > FLT_EPSILON) {
		DrawDownsample::Draw(dlist, spr, rp, ds);
	} else {
		ret = spr->GetSymbol()->DrawTree(dlist, rp, spr);
	}
	if (AFTER_SPR) {
		AFTER_SPR(spr, rp);
	}

	return ret;
}

bool DrawNode::IsSmall(const Sprite* spr, 
	                   const Actor* actor, 
	                   int min_edge)
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