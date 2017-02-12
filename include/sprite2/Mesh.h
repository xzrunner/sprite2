#ifndef _SPRITE2_MESH_H_
#define _SPRITE2_MESH_H_

#include <CU_RefCountObj.h>
#include <CU_Uncopyable.h>
#include <SM_Vector.h>

#include <vector>

namespace pm { class Mesh; }

namespace s2
{

class Symbol;

class Mesh : public cu::RefCountObj, private cu::Uncopyable
{
public:
	Mesh();
	Mesh(const Symbol* base);	
	~Mesh();

	const Symbol* GetBaseSymbol() const { return m_base; }

	float GetWidth() const { return m_width; }
	float GetHeight() const { return m_height; }

	float GetNodeRadius() const { return m_node_radius; }

	void DumpToTriangles(std::vector<sm::vec2>& vertices, std::vector<sm::vec2>& texcoords,
		std::vector<int>& triangles) const;

private:
	const Symbol* m_base;
	float m_width, m_height;	

	pm::Mesh* m_mesh;

	float m_node_radius;

}; // Mesh

}

#endif // _SPRITE2_MESH_H_