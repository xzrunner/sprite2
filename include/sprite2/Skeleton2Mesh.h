#ifndef _SPRITE2_SKELETON2_MESH_H_
#define _SPRITE2_SKELETON2_MESH_H_

#include "Mesh.h"

#include <SM_Vector.h>

#include <vector>

namespace s2
{

class MeshNode;

class Skeleton2Mesh : public VIRTUAL_INHERITANCE Mesh
{
public:
	Skeleton2Mesh();
	Skeleton2Mesh(const Skeleton2Mesh& mesh);
	Skeleton2Mesh(const Symbol* base);
	Skeleton2Mesh& operator = (const Skeleton2Mesh& mesh);

	virtual MeshType Type() const { return MESH_SKELETON2; }

	virtual void Update(const rg_skeleton_pose* sk_pose);
	virtual void Update(const rg_tl_deform_state* deform_state, const float* vertices);

	/**
	 *  @interface
	 *    Cloneable
	 */
	virtual Skeleton2Mesh* Clone() const { return new Skeleton2Mesh(*this); }

public:
	struct Item
	{
		int joint;
		sm::vec2 vertex, offset;
		float weight;
	};

	struct Vertex
	{
		std::vector<int> items;		
	};

	void SetData(const std::vector<Item>& items, const std::vector<Vertex>& vertices, 
		const std::vector<sm::vec2>& texcoords, const std::vector<int>& triangles);

	const std::vector<Item>& GetItems() const { return m_items; }
	const std::vector<Vertex>& GetVertices() const { return m_vertices; }
	const std::vector<sm::vec2>& GetTexcoords() const { return m_texcoords; }
	const std::vector<int>& GetTriangles() const { return m_triangles; }

private:
	
//	std::vector<SkinnedVertex> m_vertices;

	std::vector<Item> m_items;
	std::vector<Vertex> m_vertices;
 	std::vector<sm::vec2> m_texcoords;
 	std::vector<int> m_triangles;

	std::vector<MeshNode*> m_nodes;

	VI_DUMMY_FUNC

}; // Skeleton2Mesh

}

#endif // _SPRITE2_SKELETON2_MESH_H_