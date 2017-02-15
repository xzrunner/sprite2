#ifndef _SPRITE2_MESH_H_
#define _SPRITE2_MESH_H_

#include <CU_RefCountObj.h>
#include <CU_Uncopyable.h>
#include <SM_Vector.h>
#include <SM_Rect.h>

#include <vector>

namespace pm { class Mesh; class MeshTransform; }

struct rg_skeleton_pose;
struct rg_tl_deform_state;

namespace s2
{

class Symbol;

class Mesh : public cu::RefCountObj, private cu::Uncopyable
{
public:
	Mesh();
	Mesh(const Symbol* base);	
	virtual ~Mesh();

	const Symbol* GetBaseSymbol() const { return m_base; }

	float GetWidth() const { return m_width; }
	float GetHeight() const { return m_height; }

	float GetNodeRadius() const { return m_node_radius; }

	void DumpToTriangles(std::vector<sm::vec2>& vertices, std::vector<sm::vec2>& texcoords,
		std::vector<int>& triangles) const;

	void LoadFromTransform(const pm::MeshTransform& transform);
	void StoreToTransforom(pm::MeshTransform& transform) const;

	sm::rect GetRegion() const;

	void Update(const rg_skeleton_pose* sk_pose);
	void Update(const rg_tl_deform_state* deform_state, const float* vertices);

	void SetMesh(pm::Mesh* mesh);
	pm::Mesh* GetMesh() { return m_mesh; }
	const pm::Mesh* GetMesh() const { return m_mesh; }

protected:
	pm::Mesh* m_mesh;

private:
	const Symbol* m_base;
	float m_width, m_height;	

	float m_node_radius;

}; // Mesh

}

#endif // _SPRITE2_MESH_H_