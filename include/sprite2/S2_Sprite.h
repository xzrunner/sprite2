#ifndef _SPRITE2_SPRITE_H_
#define _SPRITE2_SPRITE_H_

#include "pre_defined.h"
#include "S2_Message.h"
#include "s2_macro.h"
#include "VisitResult.h"
#include "SprActors.h"

#include <SM_Vector.h>
#include S2_MAT_HEADER
#include <CU_RefCountObj.h>
#include <CU_Cloneable.h>

#include <string>

#include <stdint.h>

namespace s2
{

class UpdateParams;
class Symbol;
class BoundingBox;
class SprGeo;
class SprRender;
class RenderColor;
class RenderShader;
class RenderCamera;
class SpriteVisitor;
class Actor;
class SprVisitorParams;
class SprSRT;

class Sprite : public cu::RefCountObj, public cu::Cloneable
{
public:
	Sprite();
	Sprite(const Sprite& spr);
	Sprite& operator = (const Sprite& spr);
	Sprite(Symbol* sym, uint32_t id = -1);
	virtual ~Sprite();
	
	virtual void Retain(const Actor* actor) const;
	virtual void Release(const Actor* actor) const;

	virtual void OnMessage(const UpdateParams& up, Message msg) {}
	
 	virtual bool Update(const UpdateParams& up) { return false; }

	virtual bool NeedAutoUpdate(const Actor* actor) const { return false; }
	virtual bool AutoUpdate(const Actor* actor) { return false; }

	virtual Sprite* FetchChildByName(int name, const Actor* actor) const { return NULL; }
	virtual Sprite* FetchChildByIdx(int idx, const Actor* actor) const { return NULL; }

	virtual void SetSymbol(Symbol* sym);

	virtual void SetCenter(const sm::vec2& pos);
	virtual void SetPosition(const sm::vec2& pos);
	virtual void SetAngle(float angle);
	virtual void SetScale(const sm::vec2& scale);
	virtual void SetShear(const sm::vec2& shear);
	virtual void SetOffset(const sm::vec2& offset);

	virtual VisitResult TraverseChildren(SpriteVisitor& visitor, const SprVisitorParams& params) const { return VISIT_OVER; }

	static void InitHook(void (*init_flags)(Sprite* spr));

	static int GetAllSprCount();

public:

	/**
	 *  @interface
	 *    cu::Cloneable
	 *  @note
	 *    should after other virtual
	 */
	virtual Sprite* Clone() const { return NULL; }

	VisitResult Traverse(SpriteVisitor& visitor, const SprVisitorParams& params, bool init_mat = true) const;

	Symbol* GetSymbol() { return m_sym; }
	const Symbol* GetSymbol() const { return m_sym; }

	int  GetName() const { return m_name; }
	void SetName(const std::string& name);

	const BoundingBox* GetBounding(const Actor* actor = NULL) const; 
	void UpdateBounding(const Actor* actor = NULL) const;

	void Translate(const sm::vec2& trans);
	void Rotate(float rot);
	void Scale(const sm::vec2& scale);

	const sm::vec2& GetCenter() const;
	const sm::vec2& GetPosition() const;
	float	 GetAngle() const;
	const sm::vec2& GetScale() const;
	const sm::vec2& GetShear() const;
	const sm::vec2& GetOffset() const;

	const RenderColor&	GetColor() const;
	const RenderShader& GetShader() const;
	const RenderCamera& GetCamera() const;

	void SetColor(const RenderColor& color);
	void SetShader(const RenderShader& shader);
	void SetCamera(const RenderCamera& camera);

	void GetLocalSRT(SprSRT& srt) const;
	void SetLocalSRT(const SprSRT& srt);

	const S2_MAT& GetLocalMat() const;
#ifdef S2_SPR_CACHE_LOCAL_MAT_SHARE
	void CacheLocalMat();
#endif // S2_SPR_CACHE_LOCAL_MAT_SHARE

	int GetID() const { return m_id; }

	void AddActor(Actor* actor) const;
	void DelActor(Actor* actor) const;
	const Actor* QueryActor(const Actor* prev) const {
		return m_actors ? m_actors->Query(prev) : NULL;
	}
	bool HaveActor() const { return m_actors && !m_actors->IsEmpty(); }
	int ActorCount() const { return m_actors ? m_actors->Size() : 0; }
	void ClearActors() const;
	void ConnectActors(const Actor* parent) const;

private:
	void InitFlags();

	void InitFromSpr(const Sprite& spr);

	void UpdateInheritUpdate() const;

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
	S2_FLAG_METHOD(Visible, FLAG_VISIBLE)
	S2_FLAG_METHOD(Editable, FLAG_EDITABLE)
	S2_FLAG_METHOD(Dirty, FLAG_DIRTY)
	S2_FLAG_METHOD(BoundingDirty, FLAG_BOUNDING_DIRTY)
	S2_FLAG_METHOD(Integrate, FLAG_INTEGRATE)
#ifdef S2_SPR_CACHE_LOCAL_MAT_SHARE
	S2_FLAG_METHOD(GeoMatrix, FLAG_GEO_MATRIX)
#endif // S2_SPR_CACHE_LOCAL_MAT_SHARE

//	S2_FLAG_METHOD(ForceUpdate, FLAG_FORCE_UPDATE)
	bool IsForceUpdate() const { return (m_flags & FLAG_FORCE_UPDATE) != 0; }
	void SetForceUpdate(bool flag) const;

	S2_FLAG_METHOD(InheritUpdate, FLAG_INHERIT_UPDATE)

	S2_FLAG_METHOD(MatDisable, FLAG_MAT_DISABLE)
	S2_FLAG_METHOD(ColorDisable, FLAG_COLOR_DISABLE)

	// actor
	S2_FLAG_METHOD(NeedActor, FLAG_NEED_ACTOR)
	S2_FLAG_METHOD(NeedActorForChild, FLAG_NEED_ACTOR_FOR_CHILD)

	// dtex
	S2_FLAG_METHOD(DTexDisable, FLAG_DTEX_DISABLE)
	S2_FLAG_METHOD(DTexForceCached, FLAG_DTEX_FORCE_CACHED)
	S2_FLAG_METHOD(DTexForceCachedDirty, FLAG_DTEX_FORCE_CACHED_DIRTY)
	S2_FLAG_METHOD(DTexCacheBegin, FLAG_DTEX_CACHE_BEGIN)
	
	bool GetUserFlag(uint32_t key) const;
	void SetUserFlag(uint32_t key, bool val) const;

protected:
	Symbol*					m_sym;

	/************************************************************************/
	/* info                                                                 */
	/************************************************************************/
	int                     m_name;

	/************************************************************************/
	/* geometry                                                             */
	/************************************************************************/
	mutable SprGeo*         m_geo;
	mutable BoundingBox*	m_bounding;

	/************************************************************************/
	/* draw                                                                 */
	/************************************************************************/
	SprRender*              m_render;

	/************************************************************************/
	/* extend                                                               */
	/************************************************************************/
	mutable uint32_t		m_flags;

	mutable SprActors*      m_actors;

private:
	int m_id;

}; // Sprite

}

#include "S2_Sprite.inl"

#endif // _SPRITE2_SPRITE_H_