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

void IconSymbol::Draw(const RenderParams& rp, const Sprite* spr) const
{
	if (!m_icon) {
		return;
	}

	RenderParams* rp_child = RenderParamsPool::Instance()->Pop();
	*rp_child = rp;
	if (!DrawNode::Prepare(rp, spr, *rp_child)) {
		RenderParamsPool::Instance()->Push(rp_child); 
		return;
	}

	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
	sl::Sprite2Shader* shader = static_cast<sl::Sprite2Shader*>(mgr->GetShader(sl::SPRITE2));
	shader->SetColor(rp_child->color.GetMulABGR(), rp_child->color.GetAddABGR());
	shader->SetColorMap(rp_child->color.GetRMapABGR(), rp_child->color.GetGMapABGR(), rp_child->color.GetBMapABGR());

	float process = 1;
	if (spr) {
		process = VI_DOWNCASTING<const IconSprite*>(spr)->GetProcess();
	}
	m_icon->Draw(*rp_child, process);

	RenderParamsPool::Instance()->Push(rp_child); 
}

void IconSymbol::SetIcon(Icon* icon)
{
	cu::RefCountObjAssign(m_icon, icon);
}

sm::rect IconSymbol::GetBoundingImpl(const Sprite* spr, const Actor* actor, bool cache) const
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

}