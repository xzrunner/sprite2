#ifndef _SPRITE2_TRIANGLES_MESH_H_
#define _SPRITE2_TRIANGLES_MESH_H_

#include "Mesh.h"

#include <SM_Vector.h>

#include <vector>

struct rg_tl_deform_state;

namespace s2
{

class MeshNode;

class TrianglesMesh : public VIRTUAL_INHERITANCE Mesh
{
public:
	TrianglesMesh();
	TrianglesMesh(const TrianglesMesh& mesh);
	TrianglesMesh(const Symbol* base);
	TrianglesMesh& operator = (const TrianglesMesh& mesh);

	/**
	 *  @interface
	 *    Cloneable
	 */
	virtual TrianglesMesh* Clone() const { return new TrianglesMesh(*this); }

	virtual void Update(const rg_tl_deform_state* deform_state, const float* vertices);

	void SetData(const std::vector<sm::vec2>& vertices, const std::vector<sm::vec2>& texcoords,
		const std::vector<int>& triangles);
	
private:
	std::vector<sm::vec2> m_vertices;
	std::vector<sm::vec2> m_texcoords;
	std::vector<int> m_triangles;

	std::vector<MeshNode*> m_nodes;

	VI_DUMMY_FUNC

}; // TrianglesMesh

}

#endif // _SPRITE2_TRIANGLES_MESH_H_