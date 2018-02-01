#ifndef _SPRITE2_SPRITE_H_
#define _SPRITE2_SPRITE_H_

#include "sprite2/pre_defined.h"
#include "sprite2/Message.h"
#include "sprite2/macro.h"
#include "sprite2/typedef.h"
#include "sprite2/VisitResult.h"
#include "sprite2/SprActors.h"
#include "sprite2/SprRender.h"
#include "sprite2/SprDefault.h"

#include S2_MAT_HEADER
#include <cu/cu_macro.h>
#include <SM_Vector.h>
#include <painting2/BoundingBox.h>

#include <bitset>
#include <array>

#include <stdint.h>

namespace pt2 { class GeoTransform; class BoundingBox; }

namespace s2
{

class UpdateParams;
class Symbol;
class RenderShader;
class SpriteVisitor;
class SpriteVisitor2;
class Actor;
class SprVisitorParams;
class SprVisitorParams2;
class SprSRT;
class SprComponent;

class Sprite : public std::enable_shared_from_this<Sprite>
{
public:
	Sprite();
	Sprite(const Sprite& spr);
	Sprite& operator = (const Sprite& spr);
	Sprite(const SymPtr& sym, uint32_t id = -1);
	virtual ~Sprite();
	
	SprPtr Clone() const;

	virtual void OnMessage(const UpdateParams& up, Message msg) {}
	
 	virtual bool Update(const UpdateParams& up) { return false; }

	virtual bool NeedAutoUpdate(const Actor* actor) const { return false; }
	virtual bool AutoUpdate(const Actor* actor) { return false; }

	virtual SprPtr FetchChildByName(int name, const ActorConstPtr& actor) const { return nullptr; }
	virtual SprPtr FetchChildByIdx(int idx, const ActorPtr& actor) const { return nullptr; }

	virtual void SetSymbol(const SymPtr& sym);

	virtual void SetCenter(const sm::vec2& pos);
	virtual void SetPosition(const sm::vec2& pos);
	virtual void SetAngle(float angle);
	virtual void SetScale(const sm::vec2& scale);
	virtual void SetShear(const sm::vec2& shear);
	virtual void SetOffset(const sm::vec2& offset);

	virtual VisitResult TraverseChildren(SpriteVisitor& visitor, const SprVisitorParams& params) const { return VISIT_OVER; }
	virtual VisitResult TraverseChildren2(SpriteVisitor2& visitor, const SprVisitorParams2& params) const { return VISIT_OVER; }

//	static void InitHook(void (*init_flags)(const SprPtr& spr));

	static int GetAllSprCount();

public:
	VisitResult Traverse(SpriteVisitor& visitor, const SprVisitorParams& params, bool init_mat = true) const;
	VisitResult Traverse2(SpriteVisitor2& visitor, const SprVisitorParams2& params, bool init_mat = true) const;

	SymPtr& GetSymbol() { return m_sym; }
	const SymPtr& GetSymbol() const { return m_sym; }

	int  GetName() const { return m_name; }
	void SetName(const CU_STR& name);

	const pt2::BoundingBox& GetBounding(const Actor* actor = nullptr) const; 
	void UpdateBounding(const Actor* actor = nullptr) const;

	void Translate(const sm::vec2& trans);
	void Rotate(float rot);
	void Scale(const sm::vec2& scale);

	const sm::vec2& GetCenter() const;
	const sm::vec2& GetPosition() const;
	float	 GetAngle() const;
	const sm::vec2& GetScale() const;
	const sm::vec2& GetShear() const;
	const sm::vec2& GetOffset() const;

	const pt2::RenderColorCommon& GetColorCommon() const;
	const pt2::RenderColorMap&    GetColorMap() const;

	const RenderShader& GetShader() const { return *m_render->GetShader(); }
	const RenderCamera& GetCamera() const { return *m_render->GetCamera(); }

	void SetColorCommon(const pt2::RenderColorCommon& col);
	void SetColorMap(const pt2::RenderColorMap& col);

	void SetShader(const RenderShader& shader);
	void SetCamera(const RenderCamera& camera);

	void GetLocalSRT(SprSRT& srt) const;
	void SetLocalSRT(const SprSRT& srt);

	const S2_MAT& GetLocalMat() const;
#ifdef S2_SPR_CACHE_LOCAL_MAT_SHARE
	void CacheLocalMat();
#endif // S2_SPR_CACHE_LOCAL_MAT_SHARE

	int GetID() const { return m_id; }

	void AddActor(const ActorPtr& actor) const;
	void DelActor(const ActorPtr& actor) const;
	Actor* QueryActor(const Actor* prev) const {
		return m_actors ? m_actors->Query(prev) : nullptr;
	}
	ActorPtr QueryActorRef(const Actor* prev) const {
		if (m_actors) {
			const ActorPtr* ptr = m_actors->QueryPtr(prev);
			if (ptr) {
				return *ptr;
			}
		}
		return nullptr;
	}
	bool HaveActor() const { return m_actors && !m_actors->IsEmpty(); }
	int ActorCount() const { return m_actors ? m_actors->Size() : 0; }
	void ClearActors() const;
	void ConnectActors(const ActorPtr& parent) const;

	// components

	template <typename T>
	bool HasComponent() const;

	template <typename T, typename... TArgs>
	T& AddComponent(TArgs&&... args) const;

	template <typename T>
	T& GetComponent() const;

	const std::vector<std::unique_ptr<SprComponent>>& GetAllComponents() const {
		return m_components;
	}

private:
	virtual SprPtr CloneImpl() const = 0;

	void InitFlags();

	void UpdateInheritUpdate() const;

	void CopyComponentsFrom(const Sprite& spr);

	const CompTransform& GetTransformComp() const;
	const pt2::GeoTransform& GetTransform() const;

protected:
	static const uint32_t FLAG_VISIBLE        = 0x00000001;
	static const uint32_t FLAG_EDITABLE       = 0x00000002;
	static const uint32_t FLAG_DIRTY          = 0x00000004;
	static const uint32_t FLAG_BOUNDING_DIRTY = 0x00000008;
	static const uint32_t FLAG_INTEGRATE      = 0x00000010;
#ifdef S2_SPR_CACHE_LOCAL_MAT_SHARE
	static const uint32_t FLAG_GEO_MATRIX     = 0x00000020;
#endif // S2_SPR_CACHE_LOCAL_MAT_SHARE
	static const uint32_t FLAG_FORCE_UPDATE   = 0x00000040;
	static const uint32_t FLAG_INHERIT_UPDATE = 0x00000080;

	// actor
	static const uint32_t FLAG_NEED_ACTOR           = 0x00000100;
	static const uint32_t FLAG_NEED_ACTOR_FOR_CHILD = 0x00000200;

	static const uint32_t FLAG_MAT_DISABLE    = 0x00000400;	
	static const uint32_t FLAG_COLOR_DISABLE  = 0x00000800;	

	// dtex
	static const uint32_t FLAG_DTEX_DISABLE            = 0x00001000;
	static const uint32_t FLAG_DTEX_FORCE_CACHED       = 0x00002000;
	static const uint32_t FLAG_DTEX_FORCE_CACHED_DIRTY = 0x00004000;
	static const uint32_t FLAG_DTEX_CACHE_BEGIN        = 0x00008000;
	// 0x00008000

	static const uint32_t FLAG_MAX            = 0x00008000;

public:
	CU_FLAG_METHOD(Visible, FLAG_VISIBLE)
	CU_FLAG_METHOD(Editable, FLAG_EDITABLE)
	CU_FLAG_METHOD(Dirty, FLAG_DIRTY)
	CU_FLAG_METHOD(BoundingDirty, FLAG_BOUNDING_DIRTY)
	CU_FLAG_METHOD(Integrate, FLAG_INTEGRATE)
#ifdef S2_SPR_CACHE_LOCAL_MAT_SHARE
	CU_FLAG_METHOD(GeoMatrix, FLAG_GEO_MATRIX)
#endif // S2_SPR_CACHE_LOCAL_MAT_SHARE

//	CU_FLAG_METHOD(ForceUpdate, FLAG_FORCE_UPDATE)
	bool IsForceUpdate() const { return (m_flags & FLAG_FORCE_UPDATE) != 0; }
	void SetForceUpdate(bool flag) const;

	CU_FLAG_METHOD(InheritUpdate, FLAG_INHERIT_UPDATE)

	CU_FLAG_METHOD(MatDisable, FLAG_MAT_DISABLE)
	CU_FLAG_METHOD(ColorDisable, FLAG_COLOR_DISABLE)

	// actor
	CU_FLAG_METHOD(NeedActor, FLAG_NEED_ACTOR)
	CU_FLAG_METHOD(NeedActorForChild, FLAG_NEED_ACTOR_FOR_CHILD)

	// dtex
	CU_FLAG_METHOD(DTexDisable, FLAG_DTEX_DISABLE)
	CU_FLAG_METHOD(DTexForceCached, FLAG_DTEX_FORCE_CACHED)
	CU_FLAG_METHOD(DTexForceCachedDirty, FLAG_DTEX_FORCE_CACHED_DIRTY)
	CU_FLAG_METHOD(DTexCacheBegin, FLAG_DTEX_CACHE_BEGIN)
	
	bool GetUserFlag(uint32_t key) const;
	void SetUserFlag(uint32_t key, bool val) const;

protected:
	SymPtr m_sym = nullptr;

	/************************************************************************/
	/* info                                                                 */
	/************************************************************************/
	int m_name;

	/************************************************************************/
	/* draw                                                                 */
	/************************************************************************/
	static void render_deleter(SprRender* render) {
		if (render != SprDefault::Instance()->Render()) {
			mm::AllocHelper::Delete(render);
		}
	};
	std::unique_ptr<SprRender, decltype(&render_deleter)> m_render;

	/************************************************************************/
	/* extend                                                               */
	/************************************************************************/
	mutable uint32_t m_flags;

	mutable SprActors* m_actors = nullptr;

private:
	int m_id;

	// components

	mutable std::vector<std::unique_ptr<SprComponent>> m_components;

	static const size_t MAX_COMPONENTS = 16;
	mutable std::array<uint8_t, MAX_COMPONENTS> m_component_array;
	mutable std::bitset<MAX_COMPONENTS>         m_component_bitset;

}; // Sprite

}

#include "sprite2/Sprite.inl"

#endif // _SPRITE2_SPRITE_H_