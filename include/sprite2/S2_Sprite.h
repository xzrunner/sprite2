#ifndef _SPRITE2_SPRITE_H_
#define _SPRITE2_SPRITE_H_

#include "pre_defined.h"
#include "S2_Message.h"
#include "s2_macro.h"
#include "VisitResult.h"

#include <SM_Vector.h>
#include S2_MAT_HEADER
#include <CU_RefCountObj.h>
#include <CU_Cloneable.h>

#include <string>
#include <vector>

#include <stdint.h>

namespace s2
{

class RenderParams;
class Symbol;
class BoundingBox;
class SprGeo;
class SprRender;
class RenderColor;
class RenderShader;
class RenderCamera;
class SpriteVisitor;
class Actor;
class SprTreePath;
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
	
	/**
	 *  @interface
	 *    cu::RefCountObj
	 */
	virtual void AddReference() const;
	virtual void RemoveReference() const;

	virtual void OnMessage(Message msg) {}
	
 	virtual bool Update(const RenderParams& rp) { return false; }
	virtual bool SetFrame(int frame, const SprTreePath& parent_path, bool force = false) { return false; }
	virtual Sprite* FetchChild(const std::string& name, const SprTreePath& path) const { return NULL; }
	virtual Sprite* FetchChild(int idx, const SprTreePath& path) const { return NULL; }

	virtual void SetSymbol(Symbol* sym);

	virtual void SetCenter(const sm::vec2& pos);
	virtual void SetPosition(const sm::vec2& pos);
	virtual void SetAngle(float angle);
	virtual void SetScale(const sm::vec2& scale);
	virtual void SetShear(const sm::vec2& shear);
	virtual void SetOffset(const sm::vec2& offset);

	virtual VisitResult TraverseChildren(SpriteVisitor& visitor, const SprVisitorParams& params) const { return VISIT_OVER; }

	static void InitHook(void (*init_flags)(Sprite* spr));

public:

	/**
	 *  @interface
	 *    cu::Cloneable
	 *  @note
	 *    should after other virtual
	 */
	virtual Sprite* Clone() const { return NULL; }

	VisitResult Traverse(SpriteVisitor& visitor, const SprVisitorParams& params) const;

	Symbol* GetSymbol() { return m_sym; }
	const Symbol* GetSymbol() const { return m_sym; }

	const std::string& GetName() const { return m_name; }
	void SetName(const std::string& name) { m_name = name; }

	const BoundingBox* GetBounding() const; 
	void UpdateBounding() const;

	void Translate(const sm::vec2& trans);
	void Rotate(float rot);
	void Scale(const sm::vec2& scale);

	sm::vec2 GetCenter() const;
	sm::vec2 GetPosition() const;
	float	 GetAngle() const;
	sm::vec2 GetScale() const;
	sm::vec2 GetShear() const;
	sm::vec2 GetOffset() const;

	const RenderColor&	GetColor() const;
	const RenderShader& GetShader() const;
	const RenderCamera& GetCamera() const;

	void SetColor(const RenderColor& color);
	void SetShader(const RenderShader& shader);
	void SetCamera(const RenderCamera& camera);

	void GetLocalSRT(SprSRT& srt) const;
	void SetLocalSRT(const SprSRT& srt);

	S2_MAT GetLocalMat() const;
	S2_MAT GetLocalInvMat() const;
#ifdef S2_SPR_CACHE_LOCAL_MAT_SHARE
	void CacheLocalMat();
#endif // S2_SPR_CACHE_LOCAL_MAT_SHARE

	int GetID() const { return m_id; }

	void AddActor(Actor* actor) const;
	const Actor* QueryActor(const SprTreePath& path) const;
	bool HaveActor() const { return !m_actors.empty(); }
	void ClearActors() const;
	void FixActorPath(const SprTreePath& path, const SprTreePath& new_parent);

private:
	void InitFlags();

	void InitFromSpr(const Sprite& spr);

	void UpdateCenter();

protected:
	static const uint32_t FLAG_VISIBLE        = 0x00000001;
	static const uint32_t FLAG_EDITABLE       = 0x00000002;
	static const uint32_t FLAG_DIRTY          = 0x00000004;
	static const uint32_t FLAG_BOUNDING_DIRTY = 0x00000008;
	static const uint32_t FLAG_GEO_DIRTY      = 0x00000010;
#ifdef S2_SPR_CACHE_LOCAL_MAT_SHARE
	static const uint32_t FLAG_GEO_MATRIX     = 0x00000020;
#endif // S2_SPR_CACHE_LOCAL_MAT_SHARE
	static const uint32_t FLAG_FORCE_UP_FRAME = 0x00000080;

	static const uint32_t FLAG_DTEX_DISABLE            = 0x00000100;
	static const uint32_t FLAG_DTEX_FORCE_CACHED       = 0x00000200;
	static const uint32_t FLAG_DTEX_FORCE_CACHED_DIRTY = 0x00000400;

	static const uint32_t FLAG_MAX            = 0x00008000;

public:
	S2_FLAG_METHOD(Visible, FLAG_VISIBLE)
	S2_FLAG_METHOD(Editable, FLAG_EDITABLE)
	S2_FLAG_METHOD(Dirty, FLAG_DIRTY)
	S2_FLAG_METHOD(BoundingDirty, FLAG_BOUNDING_DIRTY)
	S2_FLAG_METHOD(GeoDirty, FLAG_GEO_DIRTY)
#ifdef S2_SPR_CACHE_LOCAL_MAT_SHARE
	S2_FLAG_METHOD(GeoMatrix, FLAG_GEO_MATRIX)
#endif // S2_SPR_CACHE_LOCAL_MAT_SHARE
	S2_FLAG_METHOD(ForceUpFrame, FLAG_FORCE_UP_FRAME)

	S2_FLAG_METHOD(DTexDisable, FLAG_DTEX_DISABLE)
	S2_FLAG_METHOD(DTexForceCached, FLAG_DTEX_FORCE_CACHED)
	S2_FLAG_METHOD(DTexForceCachedDirty, FLAG_DTEX_FORCE_CACHED_DIRTY)
	
	bool GetUserFlag(uint32_t key) const;
	void SetUserFlag(uint32_t key, bool val) const;

protected:
	Symbol*					m_sym;

	/************************************************************************/
	/* info                                                                 */
	/************************************************************************/
	std::string				m_name;

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

private:
	int m_id;

	mutable std::vector<Actor*> m_actors;

}; // Sprite

}

#endif // _SPRITE2_SPRITE_H_