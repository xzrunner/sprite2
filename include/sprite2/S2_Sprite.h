#ifndef _SPRITE2_SPRITE_H_
#define _SPRITE2_SPRITE_H_

#include "pre_defined.h"
#include "S2_Message.h"

#include <SM_Vector.h>
#include <SM_Matrix.h>
#include <CU_RefCountObj.h>
#include <CU_Cloneable.h>

#include <string>

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

class Sprite : public cu::RefCountObj, public cu::Cloneable
{
public:
	Sprite();
	Sprite(const Sprite& spr);
	Sprite& operator = (const Sprite& spr);
	Sprite(Symbol* sym, uint32_t id = -1);
	virtual ~Sprite();
	
	virtual void OnMessage(Message msg) {}
	
 	virtual bool Update(const RenderParams& params) { return false; }
	virtual Sprite* FetchChild(const std::string& name) const { return NULL; }
	virtual Sprite* FetchChild(int idx) const { return NULL; }
	virtual void MountChild(const std::string& name, Sprite* child) {}

	virtual void SetSymbol(Symbol* sym);

	virtual void SetCenter(const sm::vec2& pos);
	virtual void SetPosition(const sm::vec2& pos);
	virtual void SetAngle(float angle);
	virtual void SetScale(const sm::vec2& scale);
	virtual void SetShear(const sm::vec2& shear);
	virtual void SetOffset(const sm::vec2& offset);

protected:
	virtual bool TraverseChildren(SprVisitor& visitor) const { return true; }
public:

	/**
	 *  @interface
	 *    cu::Cloneable
	 *  @note
	 *    should after other virtual
	 */
	virtual Sprite* Clone() const { return NULL; }

	bool Traverse(SprVisitor& visitor) const;

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

	bool IsVisible() const;
	void SetVisible(bool visible) const;
	bool IsEditable() const;
	void SetEditable(bool editable) const;

	bool IsDirty() const;

	sm::mat4 GetLocalMat() const;
	sm::mat4 GetLocalInvMat() const;

	void SetWorldMat(const sm::mat4& mat) const;

	static int GetCount() { return m_count; }

private:
	void InitFlags();

	void InitFromSpr(const Sprite& spr);

	bool IsBoundingDirty() const;
	void SetBoundingDirty(bool dirty) const;

	void SetDirty(bool dirty) const;

	void SetWorldDirty(bool dirty) const;

protected:
	static const uint32_t FLAG_VISIBLE        = 0x00000001;
	static const uint32_t FLAG_EDITABLE       = 0x00000002;
	static const uint32_t FLAG_DIRTY          = 0x00000004;
	static const uint32_t FLAG_BOUNDING_DIRTY = 0x00000008;
	static const uint32_t FLAG_WORLD_DIRTY    = 0x00000010;

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
	static int m_count;

}; // Sprite

}

#endif // _SPRITE2_SPRITE_H_