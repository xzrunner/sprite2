#include "S2_Mesh.h"
#include "S2_Symbol.h"

#include <polymesh/Mesh.h>
#include <polymesh/TrianglesMesh.h>
#include <polymesh/Skin2Mesh.h>
#include <rigging.h>

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
		m_mesh->Dump(vertices, texcoords, triangles);
	}
}

void Mesh::LoadFromTransform(const pm::MeshTransform& transform)
{
	if (m_mesh) {
		m_mesh->LoadFromTransform(transform);
	}
}

void Mesh::StoreToTransforom(pm::MeshTransform& transform) const
{
	if (m_mesh) {
		m_mesh->StoreToTransform(transform);
	}
}

sm::rect Mesh::GetRegion() const
{
	if (!m_mesh) {
		return sm::rect(100, 100);
	}

	sm::rect ret;
	std::vector<sm::vec2> vertices, texcoords;
	std::vector<int> triangles;
	m_mesh->Dump(vertices, texcoords, triangles);
	for (int i = 0, n = vertices.size(); i < n; ++i) {
		ret.Combine(vertices[i]);
	}
	return ret;
}

static const float* 
query_joint_world_mt(int joint_id, const void* ud)
{
	const rg_skeleton_pose* sk_pose = static_cast<const rg_skeleton_pose*>(ud);
	return sk_pose->poses[joint_id].world.m;
}

void Mesh::Update(const rg_skeleton_pose* sk_pose)
{
	if (!m_mesh) {
		return;
	}

	switch (m_mesh->Type())
	{
	case pm::MESH_SKIN2:
		static_cast<pm::Skin2Mesh*>(m_mesh)->Update(query_joint_world_mt, sk_pose);
		break;
	}
}

void Mesh::Update(const rg_tl_deform_state* deform_state, const float* vertices)
{
	if (!m_mesh) {
		return;
	}

	switch (m_mesh->Type())
	{
	case pm::MESH_TRIANGLES:
		static_cast<pm::TrianglesMesh*>(m_mesh)->Update(
			deform_state->offset0, deform_state->count0, deform_state->offset1, deform_state->count1, vertices);
		break;
	case pm::MESH_SKIN2:
		static_cast<pm::Skin2Mesh*>(m_mesh)->Update(
			deform_state->offset0, deform_state->count0, deform_state->offset1, deform_state->count1, vertices);
		break;
	}
}

void Mesh::SetMesh(pm::Mesh* mesh)
{
	if (m_mesh != mesh) {
		delete m_mesh;
		m_mesh = mesh;
	}
}

}