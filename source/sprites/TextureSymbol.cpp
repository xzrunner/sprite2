#include "sprite2/TextureSymbol.h"
#include "sprite2/SymType.h"
#include "sprite2/RenderParams.h"
#include "sprite2/Sprite.h"
#include "sprite2/DrawNode.h"
#ifndef S2_DISABLE_STATISTICS
#include "sprite2/StatSymDraw.h"
#include "sprite2/StatSymCount.h"
#endif // S2_DISABLE_STATISTICS

#include <memmgr/Allocator.h>
#include <shaderlab/Blackboard.h>
#include <shaderlab/ShaderMgr.h>
#include <shaderlab/RenderContext.h>
#include <shaderlab/Sprite2Shader.h>
#ifndef S2_DISABLE_DEFERRED
#include <cooking/DisplayList.h>
#endif // S2_DISABLE_DEFERRED

namespace s2
{

TextureSymbol::TextureSymbol()
{
#ifndef S2_DISABLE_STATISTICS
	StatSymCount::Instance()->Add(STAT_SYM_TEXTURE);
#endif // S2_DISABLE_STATISTICS
}

TextureSymbol::TextureSymbol(uint32_t id)
	: Symbol(id)
{
#ifndef S2_DISABLE_STATISTICS
	StatSymCount::Instance()->Add(STAT_SYM_TEXTURE);
#endif // S2_DISABLE_STATISTICS
}

TextureSymbol::TextureSymbol(const TextureSymbol& sym)
{
	this->operator = (sym);
}

TextureSymbol& TextureSymbol::operator = (const TextureSymbol& sym)
{
	m_polygons.reserve(sym.m_polygons.size());
	for (auto& src : sym.m_polygons) {
		m_polygons.push_back(std::unique_ptr<PolygonShape>(src->Clone()));
	}
	return *this;
}

TextureSymbol::~TextureSymbol()
{
#ifndef S2_DISABLE_STATISTICS
	StatSymCount::Instance()->Subtract(STAT_SYM_TEXTURE);
#endif // S2_DISABLE_STATISTICS

	Clear();
}

int TextureSymbol::Type() const 
{ 
	return SYM_TEXTURE; 
}

pt2::RenderReturn TextureSymbol::DrawTree(cooking::DisplayList* dlist, const RenderParams& rp, const Sprite* spr) const
{
#ifndef S2_DISABLE_STATISTICS
	StatSymDraw::Instance()->AddDrawCount(STAT_SYM_TEXTURE);
//	StatSymDraw::DrawCostCP cp(STAT_SYM_TEXTURE);
#endif // S2_DISABLE_STATISTICS

	RenderParamsProxy rp_proxy;
	RenderParams* rp_child = rp_proxy.obj;
	memcpy(rp_child, &rp, sizeof(rp));

	if (!DrawNode::Prepare(rp, spr, *rp_child)) {
		return pt2::RENDER_INVISIBLE;
	}

	auto& shader_mgr = sl::Blackboard::Instance()->GetRenderContext().GetShaderMgr();
	sl::Sprite2Shader* shader = static_cast<sl::Sprite2Shader*>(shader_mgr.GetShader(sl::SPRITE2));
	shader->SetColor(rp_child->col_common.mul.ToABGR(), rp_child->col_common.add.ToABGR());
	shader->SetColorMap(rp_child->col_map.rmap.ToABGR(), rp_child->col_map.gmap.ToABGR(), rp_child->col_map.bmap.ToABGR());

	pt2::RenderReturn ret = pt2::RENDER_OK;

	// todo shape's draw return
	for (int i = 0, n = m_polygons.size(); i < n; ++i) {
		m_polygons[i]->Draw(dlist, *rp_child);
	}

	return ret;
}

#ifndef S2_DISABLE_FLATTEN
pt2::RenderReturn TextureSymbol::DrawNode(cooking::DisplayList* dlist, const RenderParams& rp, const Sprite* spr, ft::FTList& ft, int pos) const
{
	auto& shader_mgr = sl::Blackboard::Instance()->GetRenderContext().GetShaderMgr();
	sl::Sprite2Shader* shader = static_cast<sl::Sprite2Shader*>(shader_mgr.GetShader(sl::SPRITE2));
	shader->SetColor(rp.color.GetMulABGR(), rp.color.GetAddABGR());
	shader->SetColorMap(rp.color.GetRMapABGR(), rp.color.GetGMapABGR(), rp.color.GetBMapABGR());

	// todo shape's draw return
	for (int i = 0, n = m_polygons.size(); i < n; ++i) {
		m_polygons[i]->Draw(dlist, rp);
	}

	return pt2::RENDER_OK;
}
#endif // S2_DISABLE_FLATTEN

sm::rect TextureSymbol::GetBoundingImpl(const Sprite* spr, const Actor* actor, bool cache) const
{
	sm::rect rect;
	for (size_t i = 0, n = m_polygons.size(); i < n; ++i) {
		rect.Combine(m_polygons[i]->GetBounding());
	}
	return rect;
}

void TextureSymbol::Clear()
{
	m_polygons.clear();
}

}