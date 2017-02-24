#ifndef _SPRITE2_SPRITE_H_
#define _SPRITE2_SPRITE_H_

#include "pre_defined.h"
#include "S2_Message.h"

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
class SprVisitor;
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
	
 	virtual bool Update(const RenderParams& params) { return false; }
	virtual bool SetFrame(int frame) { return false; }
	virtual Sprite* FetchChild(const std::string& name) const { return NULL; }
	virtual Sprite* FetchChild(int idx) const { return NULL; }

	virtual void SetSymbol(Symbol* sym);

	virtual void SetCenter(const sm::vec2& pos);
	virtual void SetPosition(const sm::vec2& pos);
	virtual void SetAngle(float angle);
	virtual void SetScale(const sm::vec2& scale);
	virtual void SetShear(const sm::vec2& shear);
	virtual void SetOffset(const sm::vec2& offset);

protected:
	virtual bool TraverseChildren(SprVisitor& visitor, const SprVisitorParams& params) const { return true; }
public:

	/**
	 *  @interface
	 *    cu::Cloneable
	 *  @note
	 *    should after other virtual
	 */
	virtual Sprite* Clone() const { return NULL; }

	bool Traverse(SprVisitor& visitor, const SprVisitorParams& params) const;

	Symbol* GetSymbol() { return m_sym; }
	const Symbol* GetSymbol() const { return m_sym; }

	const std::string& GetName() const { return m_name; }
	void SetName(const std::string& name) { m_name = name; }

	const BoundingBox* GetBounding() const; 
	void UpdateBounding() const;

	void Translate(const sm::vec2& trans);
	void Rotate(float rot);
	void Scale(const sm::vec2& scale);

	sm::vec2			GetCenter() const;
	const sm::vec2&		GetPosition() const;
	float		        GetAngle() const;
	const sm::vec2&		GetScale() const;
	const sm::vec2&		GetShear() const;
	const sm::vec2&		GetOffset() const;

	const RenderColor&	GetColor() const;
	const RenderShader& GetShader() const;
	const RenderCamera& GetCamera() const;

	void SetColor(const RenderColor& color);
	void SetShader(const RenderShader& shader);
	void SetCamera(const RenderCamera& camera);

	const SprSRT& GetLocalSRT() const;
	void SetLocalSRT(const SprSRT& srt);

	S2_MAT GetLocalMat() const;
	S2_MAT GetLocalInvMat() const;
#ifdef S2_SPR_CACHE_LOCAL_MAT_SHARE
	void CacheLocalMat();
#endif // S2_SPR_CACHE_LOCAL_MAT_SHARE

	int GetID() const { return m_id; }

	void AddActor(Actor* actor) const;
	const Actor* QueryActor(const SprTreePath& path) const;
	void ClearActors() const;
	void FixActorPath(const SprTreePath& parent_path);
	const Sprite* GetProxy(const SprTreePath& parent_path) const;

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
	static const uint32_t FLAG_HAS_PROXY      = 0x00000040;
	static const uint32_t FLAG_FORCE_UP_FRAME = 0x00000080;

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
	FLAG_METHOD(Editable, FLAG_EDITABLE)
	FLAG_METHOD(Dirty, FLAG_DIRTY)
	FLAG_METHOD(BoundingDirty, FLAG_BOUNDING_DIRTY)
	FLAG_METHOD(GeoDirty, FLAG_GEO_DIRTY)
#ifdef S2_SPR_CACHE_LOCAL_MAT_SHARE
	FLAG_METHOD(GeoMatrix, FLAG_GEO_MATRIX)
#endif // S2_SPR_CACHE_LOCAL_MAT_SHARE
	FLAG_METHOD(HasProxy, FLAG_HAS_PROXY)
	FLAG_METHOD(ForceUpFrame, FLAG_FORCE_UP_FRAME)

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