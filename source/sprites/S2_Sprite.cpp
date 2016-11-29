#include "S2_Sprite.h"
#include "S2_Symbol.h"
#include "OBB.h"
#include "RenderFilter.h"
#include "FilterFactory.h"
#include "SprGeo.h"
#include "SprRender.h"
#include "SprDefault.h"
#include "RenderColor.h"
#include "RenderShader.h"
#include "RenderCamera.h"

#include <assert.h>

namespace s2
{

Sprite::Sprite()
	: m_sym(NULL)
	, m_geo(SprDefault::Instance()->Geo())
	, m_bounding(new OBB())
	, m_bounding_dirty(true)
	, m_render(SprDefault::Instance()->Render())
	, m_visible(true)
	, m_editable(true)
	, m_id(-1)
{
}

Sprite::Sprite(const Sprite& spr)
	: m_sym(NULL)
	, m_geo(SprDefault::Instance()->Geo())
	, m_bounding(NULL)
	, m_bounding_dirty(true)
	, m_render(SprDefault::Instance()->Render())
	, m_visible(true)
	, m_editable(true)
	, m_id(0)
{
	InitFromSpr(spr);
}

Sprite& Sprite::operator = (const Sprite& spr)
{
	InitFromSpr(spr);
	return *this;
}

Sprite::Sprite(Symbol* sym, uint32_t id)
	: m_sym(NULL)
	, m_geo(SprDefault::Instance()->Geo())
	, m_bounding(new OBB())
	, m_bounding_dirty(true)
	, m_render(SprDefault::Instance()->Render())
	, m_visible(true)
	, m_editable(true)
	, m_id(id)
{
	cu::RefCountObjAssign(m_sym, sym);
}

Sprite::~Sprite()
{
	if (m_sym) {
		m_sym->RemoveReference();
	}

	if (m_geo != SprDefault::Instance()->Geo()) {
		delete m_geo;
	}

	delete m_bounding;

	if (m_render != SprDefault::Instance()->Render()) {
		delete m_render;
	}
}

void Sprite::SetSymbol(Symbol* sym)
{
	cu::RefCountObjAssign(m_sym, sym);
	UpdateBounding();
}

void Sprite::SetCenter(const sm::vec2& pos)
{
	SetPosition(pos - GetCenter() + GetPosition());
}

void Sprite::SetPosition(const sm::vec2& pos)
{
	if (m_geo->m_position == pos) {
		return;
	}
	if (m_geo == SprDefault::Instance()->Geo()) {
		m_geo = new SprGeo;
	}

	m_geo->m_position = pos;

// 	// immediately
// 	m_bounding->SetTransform(m_position, m_offset, m_angle);

	// lazy
	m_bounding_dirty = true;
}

void Sprite::SetAngle(float angle)
{
	if (m_geo->m_angle == angle) {
		return;
	}
	if (m_geo == SprDefault::Instance()->Geo()) {
		m_geo = new SprGeo;
	}

	m_geo->m_angle = angle;

// 	// immediately
// 	m_bounding->SetTransform(m_position, m_offset, m_angle);

	// lazy
	m_bounding_dirty = true;
}

void Sprite::SetScale(const sm::vec2& scale)
{
	if (m_geo->m_scale == scale) {
		return;
	}
	if (m_geo == SprDefault::Instance()->Geo()) {
		m_geo = new SprGeo;
	}

	const sm::vec2& old_scale = m_geo->m_scale;
	if (old_scale.x != 0 && old_scale.y != 0) 
	{
		sm::vec2 dscale;
		dscale.x = scale.x / m_geo->m_scale.x;
		dscale.y = scale.y / m_geo->m_scale.y;

		if (!m_geo->m_offset.IsValid()) {
			m_geo->m_offset = m_sym->GetBounding(this).Center();
		}
		sm::vec2 old_offset = m_geo->m_offset;
		sm::vec2 new_offset(m_geo->m_offset.x * dscale.x, m_geo->m_offset.y * dscale.y);
		m_geo->m_offset = new_offset;

		m_geo->m_position += old_offset - new_offset;
	}

	m_geo->m_scale = scale;

	// lazy
	m_bounding_dirty = true;
}

void Sprite::SetShear(const sm::vec2& shear)
{
	if (m_geo->m_shear == shear) {
		return;
	}
	if (m_geo == SprDefault::Instance()->Geo()) {
		m_geo = new SprGeo;
	}

	sm::mat4 mat_old, mat_new;
	mat_old.Shear(m_geo->m_shear.x, m_geo->m_shear.y);
	mat_new.Shear(shear.x, shear.y);

	if (!m_geo->m_offset.IsValid()) {
		m_geo->m_offset = m_sym->GetBounding(this).Center();
	}
	sm::vec2 offset = mat_new * m_geo->m_offset - mat_old * m_geo->m_offset;
	m_geo->m_offset += offset;
	m_geo->m_position -= offset;

	m_geo->m_shear = shear;

	// immediately
	m_bounding->SetTransform(m_geo->m_position, m_geo->m_offset, m_geo->m_angle);

	// 	// lazy
	// 	m_bounding_dirty = true; 
}

void Sprite::SetOffset(const sm::vec2& offset)
{
	if (m_geo->m_offset == offset) {
		return;
	}
	if (m_geo == SprDefault::Instance()->Geo()) {
		m_geo = new SprGeo;
	}

	if (!m_geo->m_offset.IsValid()) {
		m_geo->m_offset = m_sym->GetBounding(this).Center();
	}

	// rotate + offset -> offset + rotate	
	sm::vec2 old_center = GetCenter();
	m_geo->m_offset = offset;
	sm::vec2 new_center = GetCenter();
	m_geo->m_position += old_center - new_center;

	// immediately
	m_bounding->SetTransform(m_geo->m_position, m_geo->m_offset, m_geo->m_angle);

	// 	// lazy
	// 	m_bounding_dirty = true; 
}

const BoundingBox* Sprite::GetBounding() const 
{ 
	if (m_bounding_dirty) {
		UpdateBounding();
	}
	return m_bounding; 
}

// todo: m_sym->GetBounding too slow, should be cached
void Sprite::UpdateBounding() const
{
	if (!m_sym) {
		return;
	}

	sm::rect rect = m_sym->GetBounding(this);
	
	if (m_geo != SprDefault::Instance()->Geo() && !m_geo->m_offset.IsValid()) {
		m_geo->m_offset = rect.Center();
	}
	m_bounding->Build(rect, m_geo->m_position, m_geo->m_angle, m_geo->m_scale, 
		m_geo->m_shear, m_geo->m_offset);

	m_bounding_dirty = false;
}

void Sprite::Translate(const sm::vec2& trans) 
{ 
	SetPosition(m_geo->m_position + trans);
}

void Sprite::Rotate(float rot) 
{ 
	SetAngle(m_geo->m_angle + rot); 
}

void Sprite::Scale(const sm::vec2& scale) 
{ 
	SetScale(m_geo->m_scale * scale); 
}

sm::vec2 Sprite::GetCenter() const
{
	if (m_geo == SprDefault::Instance()->Geo()) {
		return sm::vec2(0, 0);
	} else {
		if (!m_geo->m_offset.IsValid()) {
			m_geo->m_offset = m_sym->GetBounding(this).Center();
		}
		sm::vec2 center_offset = sm::rotate_vector(-m_geo->m_offset, m_geo->m_angle) + m_geo->m_offset;
		sm::vec2 center = m_geo->m_position + center_offset;
		return center;
	}
}

const sm::vec2& Sprite::GetPosition() const	
{ 
	return m_geo->m_position;
}

const float& Sprite::GetAngle() const
{ 
	return m_geo->m_angle;
}

const sm::vec2&	Sprite::GetScale() const
{
	return m_geo->m_scale;
}

const sm::vec2&	Sprite::GetShear() const
{
	return m_geo->m_shear;
}

const RenderColor& Sprite::GetColor() const
{
	if (!m_render || (m_render && !m_render->m_color)) {
		return *SprDefault::Instance()->Render()->m_color;
	} else {
		return *m_render->m_color;
	}
}

const RenderShader& Sprite::GetShader() const
{
	if (!m_render || (m_render && !m_render->m_shader)) {
		return *SprDefault::Instance()->Render()->m_shader;
	} else {
		return *m_render->m_shader;
	}
}

const sm::vec2& Sprite::GetOffset() const
{ 
	if (m_geo == SprDefault::Instance()->Geo()) {
		m_geo = new SprGeo;
	}
	if (!m_geo->m_offset.IsValid()) {
		m_geo->m_offset = m_sym->GetBounding(this).Center();
	}
	return m_geo->m_offset;
}

const RenderCamera& Sprite::GetCamera() const
{
	if (!m_render || (m_render && !m_render->m_camera)) {
		return *SprDefault::Instance()->Render()->m_camera;
	} else {
		return *m_render->m_camera;
	}
}

void Sprite::SetColor(const RenderColor& color)
{
	if (m_render == SprDefault::Instance()->Render() || !m_render) {
		m_render = new SprRender;
	}
	if (!m_render->m_color) {
		m_render->m_color = new RenderColor;
	}
	*m_render->m_color = color;
}

void Sprite::SetShader(const RenderShader& shader)
{
	if (m_render == SprDefault::Instance()->Render() || !m_render) {
		m_render = new SprRender;
	}
	if (!m_render->m_shader) {
		m_render->m_shader = new RenderShader;
	}
	*m_render->m_shader = shader;
}

void Sprite::SetCamera(const RenderCamera& camera)
{
	if (m_render == SprDefault::Instance()->Render() || !m_render) {
		m_render = new SprRender;
	}
	if (!m_render->m_camera) {
		m_render->m_camera = new RenderCamera;
	}
	*m_render->m_camera = camera;
}

sm::mat4 Sprite::GetTransMatrix() const
{
	if (m_geo == SprDefault::Instance()->Geo()) {
		return sm::mat4();
	} else {
		sm::vec2 center = GetCenter();
		sm::mat4 mt;
		mt.SetTransformation(center.x, center.y, m_geo->m_angle, 
			m_geo->m_scale.x, m_geo->m_scale.y, 0, 0, m_geo->m_shear.x, m_geo->m_shear.y);
		return mt;
	}
}

sm::mat4 Sprite::GetTransInvMatrix() const
{
	if (m_geo == SprDefault::Instance()->Geo()) {
		return sm::mat4();
	} else {
		sm::mat4 mat;
		mat.RotateZ(-m_geo->m_angle * SM_RAD_TO_DEG);
		mat.Shear(-m_geo->m_shear.x, -m_geo->m_shear.y);
		mat.Translate(-m_geo->m_position.x/m_geo->m_scale.x, -m_geo->m_position.y/m_geo->m_scale.y, 0);
		mat.Scale(1/m_geo->m_scale.x, 1/m_geo->m_scale.y, 1);
		return mat;
	}
}

void Sprite::InitFromSpr(const Sprite& spr)
{
	cu::RefCountObjAssign(m_sym, const_cast<Symbol*>(spr.m_sym));

	m_name = spr.m_name;

	if (m_geo != spr.m_geo) 
	{
		if (m_geo) {
			delete m_geo;
		}
		if (spr.m_geo == SprDefault::Instance()->Geo()) {
			m_geo = SprDefault::Instance()->Geo();
		} else {
			m_geo = new SprGeo(*spr.m_geo);
		}
	}

	assert(spr.m_bounding);
	if (m_bounding) {
		*m_bounding	 = *spr.m_bounding;
	} else {
		m_bounding	 = spr.m_bounding->Clone();
	}
	m_bounding_dirty = spr.m_bounding_dirty;

	if (spr.m_render != spr.m_render) 
	{
		if (m_render) {
			delete m_render;
		}
		if (spr.m_render == SprDefault::Instance()->Render()) {
			m_render = SprDefault::Instance()->Render();
		} else {
			m_render = new SprRender;
			if (spr.m_render->m_color) {
				m_render->m_color = new RenderColor(*spr.m_render->m_color);
			}
			if (spr.m_render->m_shader) {
				m_render->m_shader = new RenderShader(*spr.m_render->m_shader);
			}
			if (spr.m_render->m_camera) {
				m_render->m_camera = new RenderCamera(*spr.m_render->m_camera);
			}
		}
	}

	m_visible = spr.m_visible;
	m_editable = spr.m_editable;
}

}