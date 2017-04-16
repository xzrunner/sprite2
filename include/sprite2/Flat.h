#ifndef _SPRITE2_FLAT_H_
#define _SPRITE2_FLAT_H_

#include <SM_Matrix.h>

#include <vector>

namespace s2
{

class Sprite;
class Blob;

class Flat
{
public:
	Flat();
	~Flat();

	void Add(const Sprite* spr, const sm::mat4& mat);
	void Finish();

	const std::vector<Blob*>& GetAllBlobs() const { return m_blobs; }

private:
	std::vector<Blob*> m_blobs;	
	Blob* m_curr_blob;

}; // Flat

}

#endif // _SPRITE2_FLAT_H_