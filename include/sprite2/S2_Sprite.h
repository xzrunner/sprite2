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

	virtual void SetSymbol(Symbol* sym);

	virtual void SetCenter(const sm::vec2& pos);
	virtual void SetPosition(const sm::vec2& pos);
	virtual void SetAngle(float angle);
	virtual void SetScale(const sm::vec2& scale);
	virtual void SetShear(const sm::vec2& shear);
	virtual void SetOffset(const sm::vec2& offset);

	/**
	 *  @interface
	 *    cu::Cloneable
	 *  @note
	 *    should after other virtual
	 */
	virtual Sprite* Clone() const { return NULL; }

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
	const float&		GetAngle() const;
	const sm::vec2&		GetScale() const;
	const sm::vec2&		GetShear() const;
	const sm::vec2&		GetOffset() const;

	const RenderColor&	GetColor() const;
	const RenderShader& GetShader() const;
	const RenderCamera& GetCamera() const;

	void SetColor(const RenderColor& color);
	void SetShader(const RenderShader& shader);
	void SetCamera(const RenderCamera& camera);

	bool IsVisible() const { return m_visible; }
	void SetVisible(bool visible) { m_visible = visible; }
	bool IsEditable() const { return m_editable; }
	void SetEditable(bool editable) { m_editable = editable; }

	sm::mat4 GetTransMatrix() const;
	sm::mat4 GetTransInvMatrix() const;

	int GetID() const { return m_id; }

private:
	void InitFromSpr(const Sprite& spr);

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
	mutable bool			m_bounding_dirty;

	/************************************************************************/
	/* draw                                                                 */
	/************************************************************************/
	SprRender*              m_render;

	/************************************************************************/
	/* edit                                                                 */
	/************************************************************************/
	bool					m_visible;
	bool					m_editable;

// 	/************************************************************************/
// 	/* extend                                                               */
// 	/************************************************************************/
// 	int						m_flags;
//	void*					m_ud;

private:
	uint32_t m_id;

}; // Sprite

}

#endif // _SPRITE2_SPRITE_H_