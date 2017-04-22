#include "Flatten.h"
#include "RenderParams.h"
#include "DrawNode.h"

#include <shaderlab/ShaderMgr.h>
#include <shaderlab/Sprite2Shader.h>

namespace s2
{

void Flatten::Combine(const Flatten& ft, const S2_MAT& mt) 
{
	int base = m_quads.size();

	m_quads.reserve(m_quads.size() + ft.m_quads.size());
	for (int i = 0, n = ft.m_quads.size(); i < n; ++i) 
	{
		Quad q = ft.m_quads[i];
		q.vertices[0] = mt * q.vertices[0];
		q.vertices[1] = mt * q.vertices[1];
		q.vertices[2] = mt * q.vertices[2];
		q.vertices[3] = mt * q.vertices[3];
		m_quads.push_back(q);
	}

	m_nodes.reserve(m_nodes.size() + ft.m_nodes.size());
	for (int i = 0, n = ft.m_nodes.size(); i < n; ++i) 
	{
		m_nodes.push_back(ft.m_nodes[i]);
		Node& node = m_nodes.back();
		node.mat = mt * node.mat;
		node.idx = base + node.idx;
	}
}

void Flatten::Clear() 
{ 
	m_quads.clear(); 
	m_nodes.clear();
}

void Flatten::Draw(const RenderParams& rp) const
{
	// todo: should use quad's color
	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
	sl::Sprite2Shader* shader = static_cast<sl::Sprite2Shader*>(mgr->GetShader(sl::SPRITE2));
	shader->SetColor(rp.color.GetMulABGR(), rp.color.GetAddABGR());
	shader->SetColorMap(rp.color.GetRMapABGR(),rp.color.GetGMapABGR(), rp.color.GetBMapABGR());

	sm::vec2 vertices_trans[4];
	if (m_nodes.empty())
	{
		for (int i = 0, n = m_quads.size(); i < n; ++i)
		{
			// draw quad
			const Quad& q = m_quads[i];
			vertices_trans[0] = rp.mt * q.vertices[0];
			vertices_trans[1] = rp.mt * q.vertices[1];
			vertices_trans[2] = rp.mt * q.vertices[2];
			vertices_trans[3] = rp.mt * q.vertices[3];
			shader->DrawQuad(&vertices_trans[0].x, &q.texcoords[0].x, q.tex_id);
		}
	} 
	else
	{
		int begin = 0, end = 0;
		for (int i = 0, n = m_nodes.size(); i < n; ++i)
		{
			const Node& node = m_nodes[i];
			begin = end;
			end = node.idx;
			for (int j = begin; j < end; ++j) 
			{
				// draw quad
				const Quad& q = m_quads[j];
				vertices_trans[0] = rp.mt * q.vertices[0];
				vertices_trans[1] = rp.mt * q.vertices[1];
				vertices_trans[2] = rp.mt * q.vertices[2];
				vertices_trans[3] = rp.mt * q.vertices[3];
				shader->DrawQuad(&vertices_trans[0].x, &q.texcoords[0].x, q.tex_id);
			}
			RenderParams c_rp = rp;
			c_rp.actor = node.actor;
			c_rp.mt = node.mat;
			DrawNode::Draw(node.spr, c_rp);
		}
	}
}

void Flatten::AddNode(const Sprite* spr, const Actor* actor, const S2_MAT& mat)
{
	Node node;
	node.spr = spr;
	node.actor = actor;
	node.mat = mat;
	node.idx = m_quads.size();
	m_nodes.push_back(node);
}

}