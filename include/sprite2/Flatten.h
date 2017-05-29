#ifndef _SPRITE2_FLATTEN_H_
#define _SPRITE2_FLATTEN_H_

#include "pre_defined.h"
#include S2_MAT_HEADER
#include "RenderReturn.h"
#include "s2_macro.h"

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

	RenderReturn Draw(const RenderParams& rp) const;

	void AddQuad(const ImageSymbol* img, const sm::vec2 vertices[4]);
	void AddNode(const Sprite* spr, const Actor* actor, const S2_MAT& mat);

	void UpdateTexcoords() const;

private:
	class Quad
	{
	public:
		int tex_id;
		sm::vec2 vertices[4], texcoords[4];

	public:
		Quad() {}
 		Quad(const Quad& quad);
 		Quad& operator = (const Quad& quad);

	}; // Quad

	class Node
	{
	public:
		const Sprite* spr;
		const Actor*  actor;
		S2_MAT mat;
		int idx;

	public:
		Node();
		Node(const Node& node);
		Node& operator = (const Node& node);		

	}; // Node

private:
	RenderReturn DrawQuads(int begin, int end, const RenderParams& rp, sl::Sprite2Shader* shader) const;

	void UpdateDTexC2(int begin, int end) const;

private:
	void InitFlags();

	static const uint32_t FLAG_TEXCOORDS_NEED_UPDATE = 0x00000001;

	S2_FLAG_METHOD(TexcoordsNeedUpdate, FLAG_TEXCOORDS_NEED_UPDATE)

private:
	std::vector<const ImageSymbol*> m_images;	// todo weak ptr
	mutable std::vector<Quad> m_quads;

	std::vector<Node> m_nodes;

	mutable uint32_t  m_flags;

}; // Flatten

}

#endif // _SPRITE2_FLATTEN_H_