#include "S2_Sprite.h"
#include "S2_Symbol.h"
#include "OBB.h"
#include "RenderFilter.h"
#include "FilterFactory.h"
#include "SprGeo.h"

namespace s2
{

Sprite::Sprite()
	: m_sym(NULL)
	, m_geo(&SprGeo::DEFAULT_GEO)
	, m_visible(true)
	, m_editable(true)
	, m_id(-1)
{
	m_bounding = new OBB();
	m_bounding_dirty = true;

	m_shader.filter = FilterFactory::Instance()->Create(FM_NULL);
}

Sprite::Sprite(const Sprite& spr)
	: m_sym(NULL)
	, m_geo(&SprGeo::DEFAULT_GEO)
	, m_name(spr.m_name)
	, m_color(spr.m_color)
	, m_shader(spr.m_shader)
	, m_camera(spr.m_camera)
	, m_visible(spr.m_visible)
	, m_editable(spr.m_editable)
	, m_id(spr.m_id)
{
	if (spr.m_sym) {
		spr.m_sym->AddReference();
		m_sym = spr.m_sym;
	}

	if (spr.m_geo != &SprGeo::DEFAULT_GEO) {
		m_geo = new SprGeo;
		*m_geo = *spr.m_geo;
	}

	m_bounding = spr.m_bounding->Clone();
	m_bounding_dirty = spr.m_bounding_dirty;

	FilterMode fm = FM_NULL;
	if (spr.Shader().filter) {
		fm = spr.Shader().filter->GetMode();
	}
	m_shader.filter = FilterFactory::Instance()->Create(fm);
}

Sprite& Sprite::operator = (const Sprite& spr)
{
	cu::RefCountObjAssign(m_sym, const_cast<Symbol*>(spr.m_sym));

	m_name			= spr.m_name;

	m_geo           = &SprGeo::DEFAULT_GEO;
	if (spr.m_geo != &SprGeo::DEFAULT_GEO) {
		m_geo = new SprGeo;
		*m_geo = *spr.m_geo;
	}

	if (m_bounding) {
		*m_bounding	= *spr.m_bounding;
	} else {
		m_bounding	= spr.m_bounding->Clone();
	}
	m_bounding_dirty= spr.m_bounding_dirty;

	m_color			= spr.m_color;
	m_shader		= spr.m_shader;
	m_camera		= spr.m_camera;

	m_visible		= spr.m_visible;
	m_editable		= spr.m_editable;

	m_id            = spr.m_id;

	return *this;
}

Sprite::Sprite(Symbol* sym, uint32_t id)
	: m_sym(NULL)
	, m_geo(&SprGeo::DEFAULT_GEO)
	, m_visible(true)
	, m_editable(true)
	, m_id(id)
{
	cu::RefCountObjAssign(m_sym, sym);

	m_bounding = new OBB();
	m_bounding_dirty = true;

	m_shader.filter = FilterFactory::Instance()->Create(FM_NULL);
}

Sprite::~Sprite()
{
	if (m_sym) {
		m_sym->RemoveReference();
	}

	if (m_geo != &SprGeo::DEFAULT_GEO) {
		delete m_geo; m_geo = NULL;
	}

	delete m_bounding;

	delete m_shader.filter;
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
	if (m_geo == &SprGeo::DEFAULT_GEO) {
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
	if (m_geo == &SprGeo::DEFAULT_GEO) {
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
	if (m_geo == &SprGeo::DEFAULT_GEO) {
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
	if (m_geo == &SprGeo::DEFAULT_GEO) {
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
	if (m_geo == &SprGeo::DEFAULT_GEO) {
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
	if (m_geo == &SprGeo::DEFAULT_GEO) {
		m_bounding->Build(rect, SprGeo::DEFAULT_POSITION, SprGeo::DEFAULT_ANGLE, 
			SprGeo::DEFAULT_SCALE, SprGeo::DEFAULT_SHEAR, sm::vec2(0, 0));
	} else {
		if (!m_geo->m_offset.IsValid()) {
			m_geo->m_offset = rect.Center();
		}
		m_bounding->Build(rect, m_geo->m_position, m_geo->m_angle, m_geo->m_scale, 
			m_geo->m_shear, m_geo->m_offset);
	}

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
	if (m_geo == &SprGeo::DEFAULT_GEO) {
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

const sm::vec2& Sprite::GetOffset() const
{ 
	if (m_geo == &SprGeo::DEFAULT_GEO) {
		m_geo = new SprGeo;
	}
	if (!m_geo->m_offset.IsValid()) {
		m_geo->m_offset = m_sym->GetBounding(this).Center();
	}
	return m_geo->m_offset;
}

sm::mat4 Sprite::GetTransMatrix() const
{
	if (m_geo == &SprGeo::DEFAULT_GEO) {
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
	if (m_geo == &SprGeo::DEFAULT_GEO) {
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

}