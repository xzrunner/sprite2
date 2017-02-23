#ifndef _SPRITE2_ACTOR_H_
#define _SPRITE2_ACTOR_H_

#include "pre_defined.h"
#include "SprTreePath.h"

#include <CU_RefCountObj.h>
#include <CU_Uncopyable.h>
#include <SM_Vector.h>
#include S2_MAT_HEADER

namespace s2
{

class Sprite;
class ActorGeo;
class RenderColor;

class Actor : private cu::Uncopyable
{
public:
	Actor();
	Actor(const Sprite* spr, const SprTreePath& path);
	virtual ~Actor();
	
	const Sprite* GetSpr() const { return m_spr; }
	
	const SprTreePath& GetTreePath() const { return m_path; }
	void SetTreePath(const SprTreePath& path) { m_path = path; }

	void SetPosition(const sm::vec2& pos);
	void SetAngle(float angle);
	void SetScale(const sm::vec2& scale);
	
	S2_MAT GetLocalMat() const;

	void SetColor(const RenderColor& col);
	const RenderColor& GetColor() const { return *m_color; }

	void SetProxy(Sprite* proxy);
	const Sprite* GetProxy() const { return m_proxy; }

private:
	void InitFlags();

private:
	static const uint32_t FLAG_VISIBLE = 0x00000001;

#define FLAG_METHOD(name, bit) \
	bool Is##name##() const { \
		return (m_flags & bit) != 0; \
	} \
	void Set##name##(bool flag) const { \
		if (flag) { \
			m_flags |= bit; \
		} else { \
			m_flags &= ~bit; \
		} \
	}

public:
	FLAG_METHOD(Visible, FLAG_VISIBLE)
	
private:
	const Sprite*    m_spr;
	SprTreePath      m_path;

	ActorGeo*        m_geo;
	RenderColor*     m_color;
	Sprite*          m_proxy;
	mutable uint32_t m_flags;

}; // Actor

}

#endif // _SPRITE2_ACTOR_H_