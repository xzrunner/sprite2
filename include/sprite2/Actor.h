#ifndef _SPRITE2_ACTOR_H_
#define _SPRITE2_ACTOR_H_

#include "sprite2/pre_defined.h"
#include "sprite2/typedef.h"
#include "sprite2/ActorAABB.h"
#include "sprite2/ActorDefault.h"
#include "sprite2/SprDefault.h"
#include "sprite2/ComponentsMgr.h"

#include <cu/uncopyable.h>
#include <SM_Vector.h>
#include <SM_Rect.h>
#include <memmgr/Allocator.h>
#include S2_MAT_HEADER

#include <memory>

#include <assert.h>

namespace ft { class FTList; }
namespace cooking { class DisplayList; }

namespace s2
{

class Sprite;
class RenderShader;
class SprRender;
class RenderParams;

class Actor : private cu::Uncopyable, public std::enable_shared_from_this<Actor>, public ComponentsMgr<8>
{
public:
	Actor() = delete;
	Actor(const SprConstPtr& spr, const ActorConstPtr& parent);
	virtual ~Actor();

	SprPtr GetSpr() const { return m_spr.smart_ptr.lock(); }
	const Sprite* GetSprRaw() const { return m_spr.raw_ptr; }

	ActorPtr GetParent() const { return m_parent.smart_ptr.lock(); }
	Actor*   GetParentRaw() const { return m_parent.raw_ptr; }
	void SetParent(const ActorConstPtr& parent) { m_parent.Set(parent); }

	const sm::vec2& GetPosition() const;
	float GetAngle() const;
	const sm::vec2& GetScale() const;

	void SetPosition(const sm::vec2& pos);
	void SetAngle(float angle);
	void SetScale(const sm::vec2& scale);
	
	const S2_MAT& GetLocalMat() const;
	bool IsGeoDirty() const;

	ActorAABB& GetAABB() { return m_aabb; }
	const ActorAABB& GetAABB() const { return m_aabb; }

	// todo zz
//	const RenderShader& GetShader() const { return *m_render->GetShader(); }
	// todo zz
//	const pt2::RenderCamera& GetCamera() const { return *m_render->GetCamera(); }

	// todo zz
//	void SetColor(const RenderColor& color);
	void SetShader(const RenderShader& shader);
	// todo zz
//	void SetCamera(const pt2::RenderCamera& camera);
	
#ifndef S2_DISABLE_FLATTEN
	virtual void SetFlatten(const std::shared_ptr<ft::FTList>& ft, int pos);
	virtual void SetFlattenDirty();
	virtual void CreateFlatten();
	virtual void BuildFlatten();
	virtual bool HasFlatten() const;
	virtual void FlattenUpdate(bool force);
	virtual void FlattenDraw(const RenderParams& rp) const;
	virtual void FlattenSetFrame(int frame);
#endif // S2_DISABLE_FLATTEN

#ifndef S2_DISABLE_DEFERRED
	void BuildDisplayList();
	void SetDisplayList(const std::shared_ptr<cooking::DisplayList>& dlist);
	auto& GetDisplayList() { return m_dlist; }
#endif // S2_DISABLE_DEFERRED

	static int GetAllActorCount();

private:
	void InitFlags();

private:
	static const uint32_t FLAG_VISIBLE        = 0x00000001;
	static const uint32_t FLAG_EDITABLE       = 0x00000002;
	static const uint32_t FLAG_AABB_TIGHT     = 0x00000004;
	static const uint32_t FLAG_COLOR_DIRTY    = 0x00000008;

public:
//	CU_FLAG_METHOD(Visible, FLAG_VISIBLE)
	bool IsVisible() const { return (m_flags & FLAG_VISIBLE) != 0; }
	void SetVisible(bool flag, bool up_aabb = false) const;

	CU_FLAG_METHOD(Editable, FLAG_EDITABLE)
	CU_FLAG_METHOD(AABBTight, FLAG_AABB_TIGHT)
	CU_FLAG_METHOD(ColorDirty, FLAG_COLOR_DIRTY)
	
private:	
	const CompActorTrans& GetTransformComp() const;
	const ActorGeoTrans& GetTransform() const;

private:
	struct Flatten
	{
		std::shared_ptr<ft::FTList> list = nullptr;
		int pos = -1;
	};

	struct Spr
	{
		std::weak_ptr<Sprite> smart_ptr;
		Sprite*               raw_ptr = nullptr;

		Spr(const SprConstPtr& spr) 
			: raw_ptr(std::const_pointer_cast<Sprite>(spr).get()) 
		{
			smart_ptr = std::const_pointer_cast<Sprite>(spr);
		}

	}; // Spr

	struct Parent
	{
		std::weak_ptr<Actor> smart_ptr;
		Actor*               raw_ptr = nullptr;

		Parent(const ActorConstPtr& parent) {
			Set(parent);
		}
		void Set(const ActorConstPtr& parent) { 
			smart_ptr = std::const_pointer_cast<Actor>(parent); 
			raw_ptr = smart_ptr.lock().get();
		}

	}; // Parent

private:
	Spr m_spr;

	Parent m_parent;

	

	mutable ActorAABB m_aabb;

	// todo zz
	//static void render_deleter(SprRender* render) {
	//	if (render != SprDefault::Instance()->Render()) {
	//		mm::AllocHelper::Delete(render);
	//	}
	//};
	//std::unique_ptr<SprRender, decltype(&render_deleter)> m_render;

	mutable uint32_t m_flags;

#ifndef S2_DISABLE_FLATTEN
	Flatten m_flatten;
#endif // S2_DISABLE_FLATTEN

#ifndef S2_DISABLE_DEFERRED
	mutable std::shared_ptr<cooking::DisplayList> m_dlist = nullptr;
#endif // S2_DISABLE_DEFERRED

}; // Actor

}

#include "sprite2/Actor.inl"

#endif // _SPRITE2_ACTOR_H_