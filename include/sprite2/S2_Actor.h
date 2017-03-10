#ifndef _SPRITE2_ACTOR_H_
#define _SPRITE2_ACTOR_H_

#include "pre_defined.h"
#include "SprTreePath.h"
#include "s2_macro.h"

#include <CU_RefCountObj.h>
#include <CU_Uncopyable.h>
#include <SM_Vector.h>
#include S2_MAT_HEADER

namespace s2
{

class Sprite;
class ActorGeo;
class RenderColor;
class RenderShader;
class RenderCamera;
class SprRender;

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
	sm::vec2 GetPosition() const;
	void SetAngle(float angle);
	float GetAngle() const;
	void SetScale(const sm::vec2& scale);
	sm::vec2 GetScale() const;
	
	S2_MAT GetLocalMat() const;

	const RenderColor&	GetColor() const;
	const RenderShader& GetShader() const;
	const RenderCamera& GetCamera() const;

	void SetColor(const RenderColor& color);
	void SetShader(const RenderShader& shader);
	void SetCamera(const RenderCamera& camera);

	void SetProxy(Sprite* proxy);
	const Sprite* GetProxy() const { return m_proxy; }

private:
	void InitFlags();

private:
	static const uint32_t FLAG_VISIBLE = 0x00000001;

public:
	FLAG_METHOD(Visible, FLAG_VISIBLE)
	
private:
	const Sprite*    m_spr;
	SprTreePath      m_path;

	ActorGeo*        m_geo;
	SprRender*       m_render;
	Sprite*          m_proxy;
	mutable uint32_t m_flags;

}; // Actor

}

#endif // _SPRITE2_ACTOR_H_