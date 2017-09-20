#include "TextureSymbol.h"
#include "SymType.h"
#include "RenderParams.h"
#include "S2_Sprite.h"
#include "PolygonShape.h"
#include "DrawNode.h"
#ifndef S2_DISABLE_STATISTICS
#include "sprite2/StatSymDraw.h"
#include "sprite2/StatSymCount.h"
#endif // S2_DISABLE_STATISTICS

#include <shaderlab/ShaderMgr.h>
#include <shaderlab/Sprite2Shader.h>

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

RenderReturn TextureSymbol::Draw(const RenderParams& rp, const Sprite* spr) const
{
#ifndef S2_DISABLE_STATISTICS
	StatSymDraw::Instance()->AddDrawCount(STAT_SYM_TEXTURE);
//	StatSymDraw::DrawCostCP cp(STAT_SYM_TEXTURE);
#endif // S2_DISABLE_STATISTICS

	RenderParams* rp_child = RenderParamsPool::Instance()->Pop();
	*rp_child = rp;
	if (!DrawNode::Prepare(rp, spr, *rp_child)) {
		RenderParamsPool::Instance()->Push(rp_child); 
		return RENDER_INVISIBLE;
	}

	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
	sl::Sprite2Shader* shader = static_cast<sl::Sprite2Shader*>(mgr->GetShader(sl::SPRITE2));
	shader->SetColor(rp_child->color.GetMulABGR(), rp_child->color.GetAddABGR());
	shader->SetColorMap(rp_child->color.GetRMapABGR(), rp_child->color.GetGMapABGR(), rp_child->color.GetBMapABGR());

	RenderReturn ret = RENDER_OK;

	// todo shape's draw return
	for (int i = 0, n = m_polygons.size(); i < n; ++i) {
		m_polygons[i]->Draw(*rp_child);
	}

	RenderParamsPool::Instance()->Push(rp_child); 

	return ret;
}

RenderReturn TextureSymbol::DrawDeferred(cooking::DisplayList* dlist, 
										 const RenderParams& rp, 
										 const Sprite* spr) const
{
	RenderParams* rp_child = RenderParamsPool::Instance()->Pop();
	*rp_child = rp;
	if (!DrawNode::Prepare(rp, spr, *rp_child)) {
		RenderParamsPool::Instance()->Push(rp_child); 
		return RENDER_INVISIBLE;
	}

	RenderReturn ret = RENDER_OK;

	// todo shape's draw return
	for (int i = 0, n = m_polygons.size(); i < n; ++i) {
		m_polygons[i]->DrawDeferred(dlist, *rp_child);
	}

	RenderParamsPool::Instance()->Push(rp_child); 

	return ret;
}

void TextureSymbol::AddPolygon(PolygonShape* poly)
{
	poly->AddReference();
	m_polygons.push_back(poly);
}

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
	for_each(m_polygons.begin(), m_polygons.end(), cu::RemoveRefFunctor<PolygonShape>());
	m_polygons.clear();
}

}