#ifndef _SPRITE2_MESH_TRANSFORM2_H_
#define _SPRITE2_MESH_TRANSFORM2_H_

#include <SM_Vector.h>

#include <vector>

namespace s2
{

class Mesh;

class MeshTransform2
{
public:
	void LoadFromMesh(const Mesh* mesh);
	void StoreToMesh(const Mesh* mesh) const;

	void Lerp(const MeshTransform2& begin, const MeshTransform2& end, float process);

	void Add(int idx, const sm::vec2& trans) {
		m_trans.push_back(std::make_pair(idx, trans));
	}
	void Clear() { m_trans.clear(); }

private:
	std::vector<std::pair<int, sm::vec2> > m_trans;

	

}; // MeshTransform2

}

#endif // _SPRITE2_MESH_TRANSFORM2_H_