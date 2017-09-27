#ifndef _SPRITE2_ACTOR_H_
#define _SPRITE2_ACTOR_H_

#include "pre_defined.h"
#include "s2_macro.h"
#include "ActorAABB.h"
#include "ActorGeo.h"
#include "ActorDefault.h"

#include <CU_RefCountObj.h>
#include <CU_Uncopyable.h>
#include <SM_Vector.h>
#include <SM_Rect.h>
#include S2_MAT_HEADER

#include <memory>

namespace ft { class FTList; }
namespace cooking { class DisplayList; }

namespace s2
{

class Sprite;
class ActorGeo;
class RenderColor;
class RenderShader;
class RenderCamera;
class SprRender;
class RenderParams;

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
	const sm::vec2& GetPosition() const { return m_geo->GetPosition(); }
	void SetAngle(float angle);
	float GetAngle() const { return m_geo->GetAngle(); }
	void SetScale(const sm::vec2& scale);
	const sm::vec2& GetScale() const { return m_geo->GetScale(); }
	
	const S2_MAT& GetLocalMat() const { return m_geo->GetMatrix(); }
	bool IsGeoDirty() const { return m_geo != ActorDefault::Instance()->Geo(); }

	ActorAABB& GetAABB() { return m_aabb; }
	const ActorAABB& GetAABB() const { return m_aabb; }

	const RenderColor&	GetColor() const;
	const RenderShader& GetShader() const;
	const RenderCamera& GetCamera() const;

	void SetColor(const RenderColor& color);
	void SetShader(const RenderShader& shader);
	void SetCamera(const RenderCamera& camera);

	void SetFlatten(const std::shared_ptr<ft::FTList>& ft, int pos);
	bool SetFlattenDirty();
	bool BuildFlatten();
	bool HasFlatten() const;
	void FlattenUpdate(bool force);
	void FlattenDraw(const s2::RenderParams& rp) const;
	void FlattenSetFrame(int frame);

	void BuildDisplayList();
	void SetDisplayList(const std::shared_ptr<cooking::DisplayList>& dlist);

	static int GetAllActorCount();

private:
	void InitFlags();

private:
	static const uint32_t FLAG_VISIBLE        = 0x00000001;
	static const uint32_t FLAG_EDITABLE       = 0x00000002;
	static const uint32_t FLAG_AABB_TIGHT     = 0x00000004;

	static const uint32_t FLAG_FLATTEN_DIRTY  = 0x00000008;

	static const uint32_t FLAG_COLOR_DIRTY    = 0x00000010;

public:
//	S2_FLAG_METHOD(Visible, FLAG_VISIBLE)
	bool IsVisible() const { return (m_flags & FLAG_VISIBLE) != 0; }
	void SetVisible(bool flag, bool up_aabb = false) const;

	S2_FLAG_METHOD(Editable, FLAG_EDITABLE)
	S2_FLAG_METHOD(AABBTight, FLAG_AABB_TIGHT)
	S2_FLAG_METHOD(FlattenDirty, FLAG_FLATTEN_DIRTY)
	S2_FLAG_METHOD(ColorDirty, FLAG_COLOR_DIRTY)
	
private:
	struct Flatten
	{
		std::shared_ptr<ft::FTList> list = nullptr;
		int pos = -1;
	};

private:
	const Sprite*     m_spr;
	const Actor*      m_parent;

	ActorGeo*         m_geo;
	mutable ActorAABB m_aabb;
	SprRender*        m_render;
	mutable uint32_t  m_flags;

	Flatten           m_flatten;

	mutable std::shared_ptr<cooking::DisplayList> m_dlist = nullptr;

}; // Actor

}

#include "S2_Actor.inl"

#endif // _SPRITE2_ACTOR_H_