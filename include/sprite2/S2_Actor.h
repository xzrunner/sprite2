#ifndef _SPRITE2_ACTOR_H_
#define _SPRITE2_ACTOR_H_

#include "pre_defined.h"
#include "s2_macro.h"
#include "ActorAABB.h"

#include <CU_RefCountObj.h>
#include <CU_Uncopyable.h>
#include <SM_Vector.h>
#include <SM_Rect.h>
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
	Actor(const Sprite* spr, const Actor* parent);
	virtual ~Actor();
	
	const Sprite* GetSpr() const { return m_spr; }
	
	const Actor* GetParent() const { return m_parent; }
	void SetParent(const Actor* parent) { m_parent = parent; }

	void SetPosition(const sm::vec2& pos);
	sm::vec2 GetPosition() const;
	void SetAngle(float angle);
	float GetAngle() const;
	void SetScale(const sm::vec2& scale);
	sm::vec2 GetScale() const;
	
	S2_MAT GetLocalMat() const;

	ActorAABB& GetAABB() { return m_aabb; }
	const ActorAABB& GetAABB() const { return m_aabb; }

	const RenderColor&	GetColor() const;
	const RenderShader& GetShader() const;
	const RenderCamera& GetCamera() const;

	void SetColor(const RenderColor& color);
	void SetShader(const RenderShader& shader);
	void SetCamera(const RenderCamera& camera);

protected:
	void SetFlattenDirtyToRoot() const;

private:
	void InitFlags();

private:
	static const uint32_t FLAG_VISIBLE        = 0x00000001;
	static const uint32_t FLAG_EDITABLE       = 0x00000002;
	static const uint32_t FLAG_AABB_TIGHT     = 0x00000004;
	static const uint32_t FLAG_FLATTEN_DIRTY  = 0x00000008;

public:
	S2_FLAG_METHOD(Visible, FLAG_VISIBLE)
	S2_FLAG_METHOD(Editable, FLAG_EDITABLE)
	S2_FLAG_METHOD(AABBTight, FLAG_AABB_TIGHT)
	S2_FLAG_METHOD(FlattenDirty, FLAG_FLATTEN_DIRTY)
	
private:
	const Sprite*    m_spr;
	const Actor*     m_parent;

	ActorGeo*        m_geo;
	ActorAABB        m_aabb;
	SprRender*       m_render;
	mutable uint32_t m_flags;

}; // Actor

}

#endif // _SPRITE2_ACTOR_H_