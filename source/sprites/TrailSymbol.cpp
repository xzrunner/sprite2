#include "TrailSymbol.h"
#include "SymType.h"
#include "TrailSprite.h"
#include "RenderParams.h"
#include "S2_Sprite.h"
#include "DrawNode.h"

#include <mt_2d.h>
#include <shaderlab/ShaderMgr.h>
#include <shaderlab/Sprite2Shader.h>

namespace s2
{

TrailSymbol::TrailSymbol()
	: m_et_cfg(NULL)
	, m_et(NULL)
{
}

TrailSymbol::TrailSymbol(uint32_t id)
	: Symbol(id)
	, m_et_cfg(NULL)
	, m_et(NULL)
{
}

TrailSymbol::~TrailSymbol()
{
	if (m_et) {
		t2d_emitter_release(m_et);
	}
}

int TrailSymbol::Type() const 
{ 
	return SYM_TRAIL; 
}

void TrailSymbol::Draw(const RenderParams& rp, const Sprite* spr) const
{
	if (!spr) {
		return;
	}

	RenderParams rp_child(rp);
	rp_child.mt.x[12] = rp_child.mt.x[13] = 0;
	rp_child.color = spr->GetColor() * rp.color;

	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
	sl::Sprite2Shader* shader = static_cast<sl::Sprite2Shader*>(mgr->GetShader(sl::SPRITE2));
	shader->SetColor(rp_child.color.GetMulABGR(), rp_child.color.GetAddABGR());
	shader->SetColorMap(rp_child.color.GetRMapABGR(), rp_child.color.GetGMapABGR(), rp_child.color.GetBMapABGR());

	const TrailSprite* t2d_spr = VI_DOWNCASTING<const TrailSprite*>(spr);
	t2d_spr->Draw(rp_child);
}

sm::rect TrailSymbol::GetBounding(const Sprite* spr, const Actor* actor) const
{
#ifdef S2_ALL_SYM_SELECTED
	return sm::rect(sm::vec2(0, 0), 200, 200);
#else
	return sm::rect(0, 0);
#endif // S2_ALL_SYM_SELECTED
}

void TrailSymbol::SetEmitterCfg(t2d_emitter_cfg* cfg)
{
	if (m_et_cfg == cfg) {
		return;
	}

	m_et_cfg = cfg;

	if (m_et) {
		t2d_emitter_release(m_et);
	}
	m_et = t2d_emitter_create(m_et_cfg);
	t2d_emitter_start(m_et);
}

}