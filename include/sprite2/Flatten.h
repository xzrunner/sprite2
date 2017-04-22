#ifndef _SPRITE2_FLATTEN_H_
#define _SPRITE2_FLATTEN_H_

#include "pre_defined.h"
#include S2_MAT_HEADER

#include <SM_Vector.h>

#include <vector>

namespace s2
{

class RenderParams;

class Flatten
{
public:
	void Combine(const Flatten& ft, const S2_MAT& mt);

	void Clear() { m_list.clear(); }

	void Draw(const RenderParams& rp) const;

public:
	struct Quad
	{
		// todo: color
		uint32_t sym_id;
		int tex_id;
		sm::vec2 vertices[4], texcoords[4];
	}; // Quad

	void Add(const Quad& quad) { m_list.push_back(quad); }

private:
	std::vector<Quad> m_list;

}; // Flatten

}

#endif // _SPRITE2_FLATTEN_H_