#ifndef _SPRITE2_SKIN_H_
#define _SPRITE2_SKIN_H_

#include <CU_RefCountObj.h>
#include <CU_Uncopyable.h>

#include <SM_Vector.h>

namespace s2
{

class Sprite;

class Skin : public cu::RefCountObj, private cu::Uncopyable
{
public:
	Skin();
	~Skin();

	void Update();
	void Draw() const;

	bool Contain(const sm::vec2& pos) const;

private:
	Sprite* m_spr;

	sm::vec2 m_pos;
	float m_angle;

}; // Skin

}

#endif // _SPRITE2_SKIN_H_