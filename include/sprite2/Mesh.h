#ifndef _SPRITE2_MESH_H_
#define _SPRITE2_MESH_H_

#include "sprite2/config.h"
#include "sprite2/typedef.h"

#include <cu/uncopyable.h>
#include <cu/cu_stl.h>
#include <SM_Vector.h>
#include <SM_Rect.h>
#include <polymesh/Mesh.h>

#include <memory>

namespace pm { class MeshTransform; }

struct rg_skeleton_pose;
struct rg_tl_deform_state;

namespace s2
{

class Symbol;

class Mesh : private cu::Uncopyable
{
public:
	Mesh();
	Mesh(const SymConstPtr& base);
#ifdef S2_MESH_VIRTUAL
	virtual ~Mesh();
#else
	~Mesh();
#endif // S2_MESH_VIRTUAL

	const SymConstPtr& GetBaseSymbol() const { return m_base; }

	float GetWidth() const { return m_width; }
	float GetHeight() const { return m_height; }

	float GetNodeRadius() const { return m_node_radius; }

	void DumpToTriangles(CU_VEC<sm::vec2>& vertices, CU_VEC<sm::vec2>& texcoords,
		CU_VEC<int>& triangles) const;

	void LoadFromTransform(const pm::MeshTransform& transform);
	void StoreToTransforom(pm::MeshTransform& transform) const;

	sm::rect GetRegion() const;

	void Update(const rg_skeleton_pose* sk_pose);
	void Update(const rg_tl_deform_state* deform_state, const float* vertices);

	void SetMesh(std::unique_ptr<pm::Mesh> mesh) { m_mesh = std::move(mesh); }
	std::unique_ptr<pm::Mesh>& GetMesh() { return m_mesh; }
	const std::unique_ptr<pm::Mesh>& GetMesh() const { return m_mesh; }

protected:
	std::unique_ptr<pm::Mesh> m_mesh = nullptr;

private:
	SymConstPtr m_base = nullptr;
	float m_width, m_height;	

	float m_node_radius;

}; // Mesh

}

#endif // _SPRITE2_MESH_H_