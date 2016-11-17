#include "TrianglesMesh.h"
#include "MeshNode.h"
#include "MeshTriangle.h"

#include <assert.h>

namespace s2
{

TrianglesMesh::TrianglesMesh()
{
}

TrianglesMesh::TrianglesMesh(const TrianglesMesh& mesh)
	: Mesh(mesh)
{
	SetData(mesh.m_vertices, mesh.m_texcoords, mesh.m_triangles);
}

TrianglesMesh::TrianglesMesh(const Symbol* base)
	: Mesh(base)
{	
}

TrianglesMesh& TrianglesMesh::operator = (const TrianglesMesh& mesh)
{
	Mesh::operator = (mesh);
	SetData(mesh.m_vertices, mesh.m_texcoords, mesh.m_triangles);
	return *this;
}

void TrianglesMesh::SetData(const std::vector<sm::vec2>& vertices, 
							const std::vector<sm::vec2>& texcoords,
							const std::vector<int>& triangles)
{
	m_vertices = vertices;
	m_texcoords = texcoords;
	m_triangles = triangles;

	assert(vertices.size() == texcoords.size());
	std::vector<MeshNode*> nodes;
	nodes.reserve(vertices.size());
	for (int i = 0, n = vertices.size(); i < n; ++i) {
		MeshNode* node = new MeshNode;
		node->xy = node->ori_xy = vertices[i];
		node->uv = texcoords[i];
		nodes.push_back(node);
	}

	int ptr = 0;
	for (int i = 0, n = triangles.size() / 3; i < n; ++i)
	{
		MeshTriangle* tri = new MeshTriangle;
		for (int j = 0; j < 3; ++j) {
			tri->nodes[j] = nodes[triangles[ptr++]];
		}
		m_tris.push_back(tri);
	}
}

}