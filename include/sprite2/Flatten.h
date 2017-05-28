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
class ImageSymbol;

class Flatten
{
public:
	Flatten();

	void Combine(const Flatten& ft, const S2_MAT& mt);

	void Clear();

	void Draw(const RenderParams& rp) const;

	void AddQuad(const ImageSymbol* img, const sm::vec2 vertices[4]);
	void AddNode(const Sprite* spr, const Actor* actor, const S2_MAT& mat);

	void UpdateTexcoords() const;

private:
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

private:
	void DrawQuads(int begin, int end, const RenderParams& rp, sl::Sprite2Shader* shader) const;

	void UpdateDTexC2(int begin, int end) const;

private:
	std::vector<const ImageSymbol*> m_images;
	mutable std::vector<Quad> m_quads;

	std::vector<Node> m_nodes;

}; // Flatten

}

#endif // _SPRITE2_FLATTEN_H_