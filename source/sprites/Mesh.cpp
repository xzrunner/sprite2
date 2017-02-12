#include "Mesh.h"
#include "S2_Symbol.h"

#include <polymesh/Mesh.h>
#include <polymesh/MeshData.h>

namespace s2
{

Mesh::Mesh()
	: m_base(NULL)
	, m_width(0)
	, m_height(0)
	, m_mesh(NULL)
{
}

Mesh::Mesh(const Symbol* base)
	: m_mesh(NULL)
{
	base->AddReference();
	m_base = base;

	sm::vec2 sz = m_base->GetBounding().Size();
	m_width = sz.x;
	m_height = sz.y;

	m_node_radius = std::min(m_width * 0.1f, 5.0f);
}

Mesh::~Mesh()
{
	if (m_base) {
		m_base->RemoveReference();
	}
	if (m_mesh) {
		delete m_mesh;
	}
}

void Mesh::DumpToTriangles(std::vector<sm::vec2>& vertices, 
						   std::vector<sm::vec2>& texcoords,
						   std::vector<int>& triangles) const
{
	if (m_mesh) {
		m_mesh->GetMeshData()->Dump(vertices, texcoords, triangles);
	}
}

}