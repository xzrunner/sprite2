#include "Mesh.h"
#include "MeshTriangle.h"
#include "S2_Symbol.h"

#include <SM_Vector.h>

#include <map>

#include <assert.h>

namespace s2
{

Mesh::Mesh()
	: m_base(NULL)
	, m_width(0)
	, m_height(0)
	, m_node_radius(5)
{	
}

Mesh::Mesh(const Mesh& mesh)
	: m_width(mesh.m_width)
	, m_height(mesh.m_height)
	, m_node_radius(mesh.m_node_radius)
{
	m_base = mesh.m_base;
	if (m_base) {
		m_base->AddReference();
	}
}

Mesh::Mesh(const Symbol* base)
{
	base->AddReference();
	m_base = base;

	sm::vec2 sz = m_base->GetBounding().Size();
	m_width = sz.x;
	m_height = sz.y;

	m_node_radius = std::min(m_width * 0.1f, 5.0f);
}

Mesh& Mesh::operator = (const Mesh& mesh)
{
	m_base = mesh.m_base;
	if (m_base) {
		m_base->AddReference();
	}
	m_width = mesh.m_width;
	m_height = mesh.m_height;
	m_node_radius = mesh.m_node_radius;
	return *this;
}

Mesh::~Mesh()
{
	if (m_base) {
		m_base->RemoveReference();
	}

	ClearTriangles();
}

void Mesh::SetTriangles(const std::vector<sm::vec2>& tris)
{
	std::map<sm::vec2, MeshNode*, sm::Vector2Cmp> map2node;
	MeshNode null;
	for (int i = 0, n = tris.size(); i < n; ++i)
		map2node.insert(std::make_pair(tris[i], &null));

	for (int i = 0, n = tris.size() / 3, ptr = 0; i < n; ++i)
	{
		MeshTriangle* tri = new MeshTriangle;
		for (int j = 0; j < 3; ++j)
		{
			std::map<sm::vec2, MeshNode*, sm::Vector2Cmp>::iterator itr 
				= map2node.find(tris[ptr++]);
			assert(itr != map2node.end());
			if (itr->second == &null) {
				itr->second = new MeshNode(itr->first, m_width, m_height);
			} else {
				itr->second->AddReference();
			}
			tri->nodes[j] = itr->second;
		}
		m_tris.push_back(tri);
	}
}

sm::rect Mesh::GetRegion() const
{
	sm::rect r;
	for (int i = 0, n = m_tris.size(); i < n; ++i)
	{
		MeshTriangle* tri = m_tris[i];
		for (int i = 0; i < 3; ++i) {
			r.Combine(tri->nodes[i]->xy);
		}
	}	
	return r;
}

void Mesh::ClearTriangles()
{
	for (int i = 0, n = m_tris.size(); i < n; ++i) {
		m_tris[i]->RemoveReference();
	}
	m_tris.clear();
}

void Mesh::CopyTriangles(const Mesh& mesh)
{
	assert(m_tris.size() == mesh.m_tris.size());
	for (int i = 0, n = m_tris.size(); i < n; ++i)
	{
		MeshTriangle* src = mesh.m_tris[i];
		MeshTriangle* dst = m_tris[i];
		for (int j = 0; j < 3; ++j) {
			dst->nodes[j]->xy = src->nodes[j]->xy;
		}
	}
}

}