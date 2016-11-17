#ifndef _SPRITE2_TRIANGLES_MESH_H_
#define _SPRITE2_TRIANGLES_MESH_H_

#include "Mesh.h"

#include <SM_Vector.h>

#include <vector>

namespace s2
{

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

	void SetData(const std::vector<sm::vec2>& vertices, const std::vector<sm::vec2>& texcoords,
		const std::vector<int>& triangles);
	
private:
	std::vector<sm::vec2> m_vertices;
	std::vector<sm::vec2> m_texcoords;
	std::vector<int> m_triangles;

	VI_DUMMY_FUNC

}; // TrianglesMesh

}

#endif // _SPRITE2_TRIANGLES_MESH_H_