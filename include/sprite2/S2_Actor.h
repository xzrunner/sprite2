#ifndef _SPRITE2_ACTOR_H_
#define _SPRITE2_ACTOR_H_

#include "SprTreePath.h"

#include <CU_RefCountObj.h>
#include <CU_Uncopyable.h>

namespace s2
{

class Sprite;

class Actor : private cu::Uncopyable
{
public:
	Actor(Sprite* spr, const SprTreePath& path);
	virtual ~Actor() {}
	
	Sprite* GetSpr() { return m_spr; }
	
	const SprTreePath& GetTreePath() const { return m_path; }
	
private:
	Sprite* m_spr;

	SprTreePath m_path;

}; // Actor

}

#endif // _SPRITE2_ACTOR_H_