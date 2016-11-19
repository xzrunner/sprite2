#include "TrianglesMesh.h"
#include "MeshNode.h"
#include "MeshTriangle.h"

#include <rigging.h>

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

void TrianglesMesh::Update(const rg_tl_deform_state* state, const float* vertices)
{
	for (int i = 0; i < state->count0; ++i) {
		m_nodes[state->offset0 + i]->xy = m_nodes[state->offset0 + i]->ori_xy;
	}
	for (int i = 0; i < state->count1; ++i) {
		m_nodes[state->offset1 + i]->xy = m_nodes[state->offset1 + i]->ori_xy;
	}

	int ptr = 0;
	for (int i = 0; i < state->count0; ++i) {
		m_nodes[state->offset0 + i]->xy.x += vertices[ptr++];
		m_nodes[state->offset0 + i]->xy.y += vertices[ptr++];
	}
	for (int i = 0; i < state->count1; ++i) {
		m_nodes[state->offset1 + i]->xy.x += vertices[ptr++];
		m_nodes[state->offset1 + i]->xy.y += vertices[ptr++];
	}
}

void TrianglesMesh::SetData(const std::vector<sm::vec2>& vertices, 
							const std::vector<sm::vec2>& texcoords,
							const std::vector<int>& triangles)
{
	m_vertices = vertices;
	m_texcoords = texcoords;
	m_triangles = triangles;

	assert(vertices.size() == texcoords.size());
	m_nodes.reserve(vertices.size());
	for (int i = 0, n = vertices.size(); i < n; ++i) {
		MeshNode* node = new MeshNode;
		node->xy = node->ori_xy = vertices[i];
		node->uv = texcoords[i];
		m_nodes.push_back(node);
	}

	int ptr = 0;
	for (int i = 0, n = triangles.size() / 3; i < n; ++i)
	{
		MeshTriangle* tri = new MeshTriangle;
		for (int j = 0; j < 3; ++j) {
			tri->nodes[j] = m_nodes[triangles[ptr++]];
		}
		m_tris.push_back(tri);
	}
}

}