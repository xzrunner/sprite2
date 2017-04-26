#ifndef _SPRITE2_FLATTEN_H_
#define _SPRITE2_FLATTEN_H_

#include "pre_defined.h"
#include S2_MAT_HEADER

#include <SM_Vector.h>

#include <vector>

namespace sl { class Sprite2Shader; }

namespace s2
{

class RenderParams;
class Sprite;
class Actor;

class Flatten
{
public:
	void Combine(const Flatten& ft, const S2_MAT& mt);

	void Clear();

	void Draw(const RenderParams& rp) const;

public:
	struct Quad
	{
		int tex_id;
		sm::vec2 vertices[4], texcoords[4];
	}; // Quad

	struct Node
	{
		const Sprite* spr;
		const Actor*  actor;
		S2_MAT mat;
		int idx;
	}; // Node

	void AddQuad(const Quad& quad) { m_quads.push_back(quad); }
	void AddNode(const Sprite* spr, const Actor* actor, const S2_MAT& mat);

private:
	void DrawQuads(int begin, int end, const RenderParams& rp, sl::Sprite2Shader* shader) const;

private:
	std::vector<Quad> m_quads;
	std::vector<Node> m_nodes;

}; // Flatten

}

#endif // _SPRITE2_FLATTEN_H_