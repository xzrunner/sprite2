#include "Flatten.h"
#include "RenderParams.h"

#include <shaderlab/ShaderMgr.h>
#include <shaderlab/Sprite2Shader.h>

namespace s2
{

void Flatten::Combine(const Flatten& ft, const S2_MAT& mt) 
{
	m_list.reserve(m_list.size() + ft.m_list.size());
	for (int i = 0, n = ft.m_list.size(); i < n; ++i) 
	{
		Quad q = ft.m_list[i];
		q.vertices[0] = mt * q.vertices[0];
		q.vertices[1] = mt * q.vertices[1];
		q.vertices[2] = mt * q.vertices[2];
		q.vertices[3] = mt * q.vertices[3];
		m_list.push_back(q);
	}
}

void Flatten::Draw(const RenderParams& rp) const
{
	// todo: should use quad's color
	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
	sl::Sprite2Shader* shader = static_cast<sl::Sprite2Shader*>(mgr->GetShader(sl::SPRITE2));
	shader->SetColor(rp.color.GetMulABGR(), rp.color.GetAddABGR());
	shader->SetColorMap(rp.color.GetRMapABGR(),rp.color.GetGMapABGR(), rp.color.GetBMapABGR());

	sm::vec2 vertices_trans[4];
	for (int i = 0, n = m_list.size(); i < n; ++i)
	{
		const Quad& q = m_list[i];
		vertices_trans[0] = rp.mt * q.vertices[0];
		vertices_trans[1] = rp.mt * q.vertices[1];
		vertices_trans[2] = rp.mt * q.vertices[2];
		vertices_trans[3] = rp.mt * q.vertices[3];
		shader->DrawQuad(&vertices_trans[0].x, &q.texcoords[0].x, q.tex_id);
	}
}

}