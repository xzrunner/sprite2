#include "IconSymbol.h"
#include "SymType.h"
#include "IconSprite.h"
#include "Icon.h"
#include "RenderParams.h"
#include "DrawNode.h"

#include <shaderlab/ShaderMgr.h>
#include <shaderlab/Sprite2Shader.h>

namespace s2
{

IconSymbol::IconSymbol()
	: m_icon(NULL)
{
}

IconSymbol::IconSymbol(uint32_t id)
	: Symbol(id)
	, m_icon(NULL)
{
}

IconSymbol::~IconSymbol()
{
	if (m_icon) {
		m_icon->RemoveReference();
	}
}

int IconSymbol::Type() const 
{ 
	return SYM_ICON; 
}

void IconSymbol::Draw(const RenderParams& params, const Sprite* spr) const
{
	if (!m_icon) {
		return;
	}

	RenderParams p;
	if (!DrawNode::Prepare(params, spr, p)) {
		return;
	}

	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
	sl::Sprite2Shader* shader = static_cast<sl::Sprite2Shader*>(mgr->GetShader(sl::SPRITE2));
	shader->SetColor(p.color.GetMul().ToABGR(), p.color.GetAdd().ToABGR());
	shader->SetColorMap(p.color.GetMapR().ToABGR(), p.color.GetMapG().ToABGR(), p.color.GetMapB().ToABGR());

	float process = 1;
	if (spr) {
		process = VI_DOWNCASTING<const IconSprite*>(spr)->GetProcess();
	}
	m_icon->Draw(p, process);
}

sm::rect IconSymbol::GetBounding(const Sprite* spr) const
{
	sm::rect r;
	if (!m_icon) {
		return r;
	}

	float process = 1;
	if (spr) {
		process = VI_DOWNCASTING<const IconSprite*>(spr)->GetProcess();
	}
	return m_icon->GetRegion(process);
}

void IconSymbol::SetIcon(Icon* icon)
{
	cu::RefCountObjAssign(m_icon, icon);
}

}