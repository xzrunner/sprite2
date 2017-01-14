#include "Skeleton2Mesh.h"
#include "MeshNode.h"
#include "MeshTriangle.h"

#include <rigging.h>

#include <assert.h>

namespace s2
{

Skeleton2Mesh::Skeleton2Mesh()
{
}

Skeleton2Mesh::Skeleton2Mesh(const Skeleton2Mesh& mesh)
	: Mesh(mesh)
{
	SetData(mesh.m_items, mesh.m_vertices, mesh.m_texcoords, mesh.m_triangles);
}

Skeleton2Mesh::Skeleton2Mesh(const Symbol* base)
	: Mesh(base)
{	
}

Skeleton2Mesh& Skeleton2Mesh::operator = (const Skeleton2Mesh& mesh)
{
	Mesh::operator = (mesh);
	SetData(mesh.m_items, mesh.m_vertices, mesh.m_texcoords, mesh.m_triangles);
	return *this;
}

void Skeleton2Mesh::Update(const rg_skeleton_pose* sk_pose)
{
	std::vector<sm::vec2> vertices;
	vertices.reserve(m_vertices.size());
	for (int i = 0, n = m_vertices.size(); i < n; ++i) 
	{
		sm::vec2 v;
		for (int j = 0, m = m_vertices[i].items.size(); j < m; ++j)
		{
			const Item& item = m_items[m_vertices[i].items[j]];
			const float* mat = sk_pose->poses[item.joint].world.m;
			sm::vec2 vertex = item.vertex + item.offset;
			v.x += (vertex.x * mat[0] + vertex.y * mat[2] + mat[4]) * item.weight;
			v.y += (vertex.x * mat[1] + vertex.y * mat[3] + mat[5]) * item.weight;
		}
		vertices.push_back(v);
	}

	for (int i = 0, n = m_triangles.size(); i < n; ++i)
	{
		int tri = i / 3,
			node = i - tri * 3;
		m_tris[tri]->nodes[node]->xy = vertices[m_triangles[i]];
	}
}

void Skeleton2Mesh::Update(const rg_tl_deform_state* state, const float* vertices)
{
	for (int i = 0; i < state->count0; ++i) {
		m_items[state->offset0 + i].offset.Set(0, 0);
	}
	for (int i = 0; i < state->count1; ++i) {
		m_items[state->offset1 + i].offset.Set(0, 0);
	}

	int ptr = 0;
	for (int i = 0; i < state->count0; ++i) {
		sm::vec2 offset(vertices[ptr++], vertices[ptr++]);
		m_items[state->offset0 + i].offset += offset;
	}
	for (int i = 0; i < state->count1; ++i) {
		sm::vec2 offset(vertices[ptr++], vertices[ptr++]);
		m_items[state->offset1 + i].offset += offset;
	}
}

void Skeleton2Mesh::SetData(const std::vector<Item>& items, 
							const std::vector<Vertex>& vertices, 
							const std::vector<sm::vec2>& texcoords, 
							const std::vector<int>& triangles)
{
	m_items = items;
	m_vertices = vertices;
	m_texcoords = texcoords;
	m_triangles = triangles;

	assert(vertices.size() == texcoords.size());
	m_nodes.reserve(vertices.size());
	for (int i = 0, n = vertices.size(); i < n; ++i) {
		MeshNode* node = new MeshNode;
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