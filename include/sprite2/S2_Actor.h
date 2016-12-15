#ifndef _SPRITE2_ACTOR_H_
#define _SPRITE2_ACTOR_H_

#include "SprTreePath.h"

#include <CU_RefCountObj.h>
#include <CU_Uncopyable.h>
#include <SM_Vector.h>
#include <SM_Matrix.h>

namespace s2
{

class Sprite;
class ActorGeo;

class Actor : private cu::Uncopyable
{
public:
	Actor(const Sprite* spr, const SprTreePath& path);
	virtual ~Actor();
	
	const Sprite* GetSpr() const { return m_spr; }
	
	const SprTreePath& GetTreePath() const { return m_path; }

	void SetPosition(const sm::vec2& pos);
	void SetAngle(float angle);
	void SetScale(const sm::vec2& scale);
	
	sm::mat4 GetLocalMat() const;

private:
	const Sprite* m_spr;

	SprTreePath m_path;

	ActorGeo* m_geo;

}; // Actor

}

#endif // _SPRITE2_ACTOR_H_