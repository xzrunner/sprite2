#ifndef _SPRITE2_BLOB_H_
#define _SPRITE2_BLOB_H_

#include <SM_Matrix.h>

#include <vector>

namespace s2
{

class Sprite;
class ImageSprite;
class RenderParams;

class Blob
{
public:
	Blob();
	Blob(const Sprite* spr, const sm::mat4& mat);

	void Add(const ImageSprite* img_spr, const sm::mat4& prev_mat);

	void Draw(const RenderParams& rp) const;

private:
	struct Quad
	{
		sm::vec2 vertices[4], texcoords[4];
		int texid;
	};

private:
	const Sprite* m_spr;
	sm::mat4 m_mat;
	
	mutable std::vector<Quad> m_quads;

}; // Blob

}

#endif // _SPRITE2_BLOB_H_