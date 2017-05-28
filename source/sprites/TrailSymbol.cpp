#include "TrailSymbol.h"
#include "SymType.h"
#include "TrailSprite.h"
#include "RenderParams.h"
#include "S2_Sprite.h"
#include "DrawNode.h"
#include "TrailEmitter.h"
#include "TrailEmitterCfg.h"

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
	if (m_et_cfg) {
		m_et_cfg->RemoveReference();
	}
	if (m_et) {
		m_et->RemoveReference();
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

	RenderParams* rp_child = RenderParamsPool::Instance()->Pop();
	*rp_child = rp;

	rp_child->mt.x[4] = rp_child->mt.x[5] = 0;
	rp_child->color = spr->GetColor() * rp.color;

	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
	sl::Sprite2Shader* shader = static_cast<sl::Sprite2Shader*>(mgr->GetShader(sl::SPRITE2));
	shader->SetColor(rp_child->color.GetMulABGR(), rp_child->color.GetAddABGR());
	shader->SetColorMap(rp_child->color.GetRMapABGR(), rp_child->color.GetGMapABGR(), rp_child->color.GetBMapABGR());

	const TrailSprite* t2d_spr = VI_DOWNCASTING<const TrailSprite*>(spr);
	t2d_spr->Draw(*rp_child);

	RenderParamsPool::Instance()->Push(rp_child); 
}

void TrailSymbol::SetEmitterCfg(const TrailEmitterCfg* cfg)
{
	if (m_et_cfg == cfg) {
		return;
	}

	cu::RefCountObjAssign(m_et_cfg, cfg);
	if(!cfg) {
		return;
	}

	if (m_et) {
		m_et->RemoveReference();
	}

	m_et = TrailEmitterPool::Instance()->Pop();
	m_et->CreateEmitter(m_et_cfg);
	m_et->Start();
}

sm::rect TrailSymbol::GetBoundingImpl(const Sprite* spr, const Actor* actor, bool cache) const
{
	return sm::rect(); // empty
}

}