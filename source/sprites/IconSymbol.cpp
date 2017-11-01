#include "sprite2/IconSymbol.h"
#include "sprite2/SymType.h"
#include "sprite2/IconSprite.h"
#include "sprite2/RenderParams.h"
#include "sprite2/DrawNode.h"
#ifndef S2_DISABLE_STATISTICS
#include "sprite2/StatSymDraw.h"
#include "sprite2/StatSymCount.h"
#endif // S2_DISABLE_STATISTICS

#include <memmgr/Allocator.h>
#include <shaderlab/ShaderMgr.h>
#include <shaderlab/Sprite2Shader.h>

namespace s2
{

IconSymbol::IconSymbol()
{
#ifndef S2_DISABLE_STATISTICS
	StatSymCount::Instance()->Add(STAT_SYM_ICON);
#endif // S2_DISABLE_STATISTICS
}

IconSymbol::IconSymbol(uint32_t id)
	: Symbol(id)
{
#ifndef S2_DISABLE_STATISTICS
	StatSymCount::Instance()->Add(STAT_SYM_ICON);
#endif // S2_DISABLE_STATISTICS
}

IconSymbol::IconSymbol(const IconSymbol& sym)
	: m_icon(sym.m_icon->Clone())
{
}

IconSymbol& IconSymbol::operator = (const IconSymbol& sym)
{
	m_icon.reset(sym.m_icon->Clone());
	return *this;
}

IconSymbol::~IconSymbol()
{
#ifndef S2_DISABLE_STATISTICS
	StatSymCount::Instance()->Subtract(STAT_SYM_ICON);
#endif // S2_DISABLE_STATISTICS
}

int IconSymbol::Type() const 
{ 
	return SYM_ICON; 
}

RenderReturn IconSymbol::DrawTree(cooking::DisplayList* dlist, const RenderParams& rp, const Sprite* spr) const
{
#ifndef S2_DISABLE_STATISTICS
	StatSymDraw::Instance()->AddDrawCount(STAT_SYM_ICON);
//	StatSymDraw::DrawCostCP cp(STAT_SYM_ICON);
#endif // S2_DISABLE_STATISTICS

	if (!m_icon) {
		return RENDER_NO_DATA;
	}

	RenderParamsProxy rp_proxy;
	RenderParams* rp_child = rp_proxy.obj;
	memcpy(rp_child, &rp, sizeof(rp));

	if (!DrawNode::Prepare(rp, spr, *rp_child)) {
		return RENDER_INVISIBLE;
	}

	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
	sl::Sprite2Shader* shader = static_cast<sl::Sprite2Shader*>(mgr->GetShader(sl::SPRITE2));
	shader->SetColor(rp_child->color.GetMulABGR(), rp_child->color.GetAddABGR());
	shader->SetColorMap(rp_child->color.GetRMapABGR(), rp_child->color.GetGMapABGR(), rp_child->color.GetBMapABGR());

	float process = 1;
	if (spr) {
		process = S2_VI_DOWN_CAST<const IconSprite*>(spr)->GetProcess();
	}
	RenderReturn ret = m_icon->Draw(*rp_child, process);

	return ret;
}

#ifndef S2_DISABLE_FLATTEN
RenderReturn IconSymbol::DrawNode(cooking::DisplayList* dlist, const RenderParams& rp, const Sprite* spr, ft::FTList& ft, int pos) const
{
	if (!m_icon) {
		return RENDER_NO_DATA;
	}

	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
	sl::Sprite2Shader* shader = static_cast<sl::Sprite2Shader*>(mgr->GetShader(sl::SPRITE2));
	shader->SetColor(rp.color.GetMulABGR(), rp.color.GetAddABGR());
	shader->SetColorMap(rp.color.GetRMapABGR(), rp.color.GetGMapABGR(), rp.color.GetBMapABGR());

	float process = 1;
	if (spr) {
		process = S2_VI_DOWN_CAST<const IconSprite*>(spr)->GetProcess();
	}
	return m_icon->Draw(rp, process);
}
#endif // S2_DISABLE_FLATTEN

sm::rect IconSymbol::GetBoundingImpl(const Sprite* spr, const Actor* actor, bool cache) const
{
	sm::rect r;
	if (!m_icon) {
		return r;
	}

	float process = 1;
	if (spr) {
		process = S2_VI_DOWN_CAST<const IconSprite*>(spr)->GetProcess();
	}
	return m_icon->GetRegion(process);
}

}