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

#include <unirender/RenderContext.h>
#include <shaderlab/ShaderMgr.h>
#include <shaderlab/FilterShader.h>
#include <shaderlab/EdgeDetectProg.h>

namespace s2
{

static void (*AFTER_SPR)(const Sprite*, const RenderParams&);

void DrawNode::InitCB(void (*after_spr)(const Sprite*, const RenderParams&))
{
	AFTER_SPR = after_spr;
}

RenderParams DrawNode::Prepare(const RenderParams& parent, const Sprite* spr)
{
	if (!spr) {
		return parent;
	}

	RenderParams ret = parent;

	ret.path.Push(spr->GetID());

	ret.mt = spr->GetLocalMat() * parent.mt;
	ret.color = spr->GetColor() * parent.color;

	const Actor* actor = spr->QueryActor(ret.path);
	if (actor) {
		ret.mt = actor->GetLocalMat() * ret.mt;
	}

	return ret;
}

void DrawNode::Draw(const Sprite* spr, const RenderParams& params)
{
	if (!spr->IsVisible()) {
		return;
	}

	RenderShader rs;
	RenderCamera rc;

	if (params.disable_render) {
		rs = *SprDefault::Instance()->Shader();
		rc = *SprDefault::Instance()->Camera();
	} else {
		rs = spr->GetShader() * params.shader;
		rc = spr->GetCamera() * params.camera;
	}

	ur::RenderContext* rctx = sl::ShaderMgr::Instance()->GetContext();
	switch (rs.GetFastBlend())
	{
	case FBM_NULL:
		rctx->SetBlend(2, 6);			// BLEND_GL_ONE, BLEND_GL_ONE_MINUS_SRC_ALPHA
		rctx->SetBlendEquation(0);	// BLEND_FUNC_ADD
		break;
	case FBM_ADD:
		rctx->SetBlend(2, 2);			// BLEND_GL_ONE, BLEND_GL_ONE
		rctx->SetBlendEquation(0);	// BLEND_FUNC_ADD
		break;
	case FBM_SUBTRACT:
		rctx->SetBlend(2, 6);			// BLEND_GL_ONE, BLEND_GL_ONE_MINUS_SRC_ALPHA
		rctx->SetBlendEquation(1);	// BLEND_FUNC_SUBTRACT
		break;
	}

	BlendMode blend = BM_NULL;
	if (!params.disable_blend) {
		blend = rs.GetBlend();
	}

	FilterMode filter = FM_NULL;
	if (!params.disable_filter && rs.GetFilter()) {
		filter = rs.GetFilter()->GetMode();
	}

	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
	if (blend != BM_NULL) {
// 		const Camera* cam = CameraMgr::Instance()->GetCamera();
// 		if (cam->Type() == "ortho") {
			DrawBlend::Draw(spr, params.mt);
//		}
	} else if (filter != FM_NULL) {
		const RenderFilter* rf = rs.GetFilter();

		RenderParams t = params;
		t.shader.SetFilter(rf);
		t.camera = rc;
		if (filter == FM_GAUSSIAN_BLUR) {
 			int itrs = static_cast<const RFGaussianBlur*>(rf)->GetIterations();
 			DrawGaussianBlur::Draw(spr, t, itrs);
		} else if (filter == FM_OUTER_GLOW) {
			int itrs = static_cast<const RFOuterGlow*>(rf)->GetIterations();
			DrawOuterGlow::Draw(spr, t, itrs);
		} else {
			if (params.set_shader) {
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
			DrawSpr(spr, t);
		}
	} else {
		if (params.set_shader) {
			mgr->SetShader(sl::SPRITE2);
		}
		RenderParams t = params;
		t.camera = rc;
		DrawSpr(spr, t);
	}
}

void DrawNode::Draw(const Symbol* sym, const RenderParams& params, 
					const sm::vec2& pos, float angle, 
					const sm::vec2& scale, const sm::vec2& shear)
{
 	sm::mat4 mt;
 	mt.SetTransformation(pos.x, pos.y, angle, scale.x, scale.y, 0, 0, shear.x, shear.y);
 	mt = mt * params.mt;

 	RenderParams t = params;
 	t.mt = mt;
 
	BlendMode blend = BM_NULL;
	if (!params.disable_blend) {
		blend = params.shader.GetBlend();
	}

	FilterMode filter = FM_NULL;
	if (!params.disable_filter && params.shader.GetFilter()) {
		filter = params.shader.GetFilter()->GetMode();
	}

 	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
 	if (blend != BM_NULL) {
 		;
 	} else if (filter != FM_NULL) {
 		if (t.set_shader) {
 			mgr->SetShader(sl::FILTER);
 			sl::FilterShader* shader = static_cast<sl::FilterShader*>(mgr->GetShader());
 			shader->SetMode(sl::FILTER_MODE(filter));
 		}
 	} else {
 		if (t.set_shader) {
 			mgr->SetShader(sl::SPRITE2);
 		}
 	}
 
 	sym->Draw(t);
}

void DrawNode::Draw(const Symbol* sym, const RenderParams& params, const sm::mat4& _mt)
{
	sm::mat4 mt = _mt * params.mt;

	RenderParams t = params;
	t.mt = mt;

	BlendMode blend = BM_NULL;
	if (!params.disable_blend) {
		blend = params.shader.GetBlend();
	}

	FilterMode filter = FM_NULL;
	if (!params.disable_filter && params.shader.GetFilter()) {
		filter = params.shader.GetFilter()->GetMode();
	}

	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
	if (blend != BM_NULL) {
		;
	} else if (filter != FM_NULL) {
		if (t.set_shader) {
			mgr->SetShader(sl::FILTER);
			sl::FilterShader* shader = static_cast<sl::FilterShader*>(mgr->GetShader());
			shader->SetMode(sl::FILTER_MODE(filter));
		}
	} else {
		if (t.set_shader) {
			mgr->SetShader(sl::SPRITE2);
		}
	}

	sym->Draw(t);
}

void DrawNode::DrawSpr(const Sprite* spr, const RenderParams& params)
{
	spr->GetSymbol()->Draw(params, spr);
	if (AFTER_SPR) {
		AFTER_SPR(spr, params);
	}
}

}