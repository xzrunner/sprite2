#ifndef _SPRITE2_FLATTEN_H_
#define _SPRITE2_FLATTEN_H_

#include <SM_Vector.h>

#include <vector>

namespace s2
{

class Flatten
{
public:

	void Combine(const Flatten& ft) {
		m_list.reserve(m_list.size() + ft.m_list.size());
		m_list.insert(m_list.end(), ft.m_list.begin(), ft.m_list.end());
	}

	void Clear() { m_list.clear(); }

private:
	struct Quad
	{
		uint32_t sym_id;
		int tex_id;
		sm::vec2 vertices[4], texcoords[4];
	}; // Quad

private:
	std::vector<Quad> m_list;

}; // Flatten

}

#endif // _SPRITE2_FLATTEN_H_