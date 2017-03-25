#include "TextureSymbol.h"
#include "SymType.h"
#include "RenderParams.h"
#include "S2_Sprite.h"
#include "PolygonShape.h"
#include "DrawNode.h"

#include <shaderlab/ShaderMgr.h>
#include <shaderlab/Sprite2Shader.h>

namespace s2
{

TextureSymbol::TextureSymbol()
{
}

TextureSymbol::TextureSymbol(uint32_t id)
	: Symbol(id)
{
}

TextureSymbol::~TextureSymbol()
{
	Clear();
}

int TextureSymbol::Type() const 
{ 
	return SYM_TEXTURE; 
}

void TextureSymbol::Draw(const RenderParams& rp, const Sprite* spr) const
{
	RenderParams rp_child;
	if (!DrawNode::Prepare(rp, spr, rp_child)) {
		return;
	}

	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
	sl::Sprite2Shader* shader = static_cast<sl::Sprite2Shader*>(mgr->GetShader(sl::SPRITE2));
	shader->SetColor(rp_child.color.GetMulABGR(), rp_child.color.GetAddABGR());
	shader->SetColorMap(rp_child.color.GetRMapABGR(), rp_child.color.GetGMapABGR(), rp_child.color.GetBMapABGR());

	for (int i = 0, n = m_polygons.size(); i < n; ++i) {
		m_polygons[i]->Draw(rp_child);
	}
}

sm::rect TextureSymbol::GetBounding(const Sprite* spr, const Actor* actor) const
{
	sm::rect rect;
	for (size_t i = 0, n = m_polygons.size(); i < n; ++i) {
		rect.Combine(m_polygons[i]->GetBounding());
	}
	return rect;
}

void TextureSymbol::AddPolygon(PolygonShape* poly)
{
	poly->AddReference();
	m_polygons.push_back(poly);
}

void TextureSymbol::Clear()
{
	for_each(m_polygons.begin(), m_polygons.end(), cu::RemoveRefFunctor<PolygonShape>());
	m_polygons.clear();
}

}