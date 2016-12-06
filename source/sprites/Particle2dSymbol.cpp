#include "Particle2dSymbol.h"
#include "SymType.h"
#include "Particle2dSprite.h"
#include "RenderParams.h"

#include <ps_2d.h>
#include <shaderlab.h>

namespace s2
{

Particle2dSymbol::Particle2dSymbol()
	: m_et_cfg(NULL)
{
	Particle2d::Instance();
}

Particle2dSymbol::Particle2dSymbol(uint32_t id)
	: Symbol(id)
	, m_et_cfg(NULL)
{
	Particle2d::Instance();
}

Particle2dSymbol::~Particle2dSymbol()
{
}

int Particle2dSymbol::Type() const 
{ 
	return SYM_PARTICLE2D; 
}

void Particle2dSymbol::Draw(const RenderParams& params, const Sprite* spr) const
{
	if (!spr) {
		return;
	}

	const Particle2dSprite* p2d_spr = VI_DOWNCASTING<const Particle2dSprite*>(spr);

	RenderParams p = params;
	p.mt = p2d_spr->GetLocalMat() * params.mt;
	p2d_spr->SetWorldMat(p.mt);
	p.color = spr->GetColor() * params.color;

	p2d_spr->SetMatrix(p.mt);

	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
	sl::Sprite2Shader* shader = static_cast<sl::Sprite2Shader*>(mgr->GetShader(sl::SPRITE2));
	shader->SetColor(p.color.mul.ToABGR(), p.color.add.ToABGR());
	shader->SetColorMap(p.color.rmap.ToABGR(), p.color.gmap.ToABGR(), p.color.bmap.ToABGR());

	p2d_spr->Draw(p);	
}

sm::rect Particle2dSymbol::GetBounding(const Sprite* spr) const
{
	return sm::rect(sm::vec2(0, 0), 200, 200);
}

}