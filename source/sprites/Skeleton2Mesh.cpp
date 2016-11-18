#include "Skeleton2Mesh.h"
#include "MeshNode.h"
#include "MeshTriangle.h"

#include <rigging/rg_skeleton_pose.h>
#include <rigging/rg_pose_mat.h>
#include <rigging/rg_joint.h>

#include <assert.h>

namespace s2
{

Skeleton2Mesh::Skeleton2Mesh()
{
}

Skeleton2Mesh::Skeleton2Mesh(const Skeleton2Mesh& mesh)
	: Mesh(mesh)
{
	SetData(mesh.m_vertices, mesh.m_texcoords, mesh.m_triangles);
}

Skeleton2Mesh::Skeleton2Mesh(const Symbol* base)
	: Mesh(base)
{	
}

Skeleton2Mesh& Skeleton2Mesh::operator = (const Skeleton2Mesh& mesh)
{
	Mesh::operator = (mesh);
	SetData(mesh.m_vertices, mesh.m_texcoords, mesh.m_triangles);
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
			const SkinnedVertex::Item& item = m_vertices[i].items[j];
			const float* mat = sk_pose->poses[item.joint].world.m;
			v.x += (item.vx * mat[0] + item.vy * mat[2] + mat[4]) * item.weight;
			v.y += (item.vx * mat[1] + item.vy * mat[3] + mat[5]) * item.weight;
// 			v.x += (item.vx) * item.weight;
// 			v.y += (item.vy) * item.weight;
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

void Skeleton2Mesh::SetData(const std::vector<SkinnedVertex>& vertices, 
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