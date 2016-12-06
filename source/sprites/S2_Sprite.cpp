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
#include "SprVisitor.h"

#include <assert.h>

namespace s2
{

int Sprite::m_count = 0;

Sprite::Sprite()
	: m_sym(NULL)
	, m_geo(SprDefault::Instance()->Geo())
	, m_bounding(new OBB())
	, m_render(SprDefault::Instance()->Render())
	, m_flags(0)
{
	++m_count;

	InitFlags();
}

Sprite::Sprite(const Sprite& spr)
	: m_sym(NULL)
	, m_geo(SprDefault::Instance()->Geo())
	, m_bounding(NULL)
	, m_render(SprDefault::Instance()->Render())
	, m_flags(0)
{
	++m_count;

	InitFromSpr(spr);
	InitFlags();
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
	, m_render(SprDefault::Instance()->Render())
	, m_flags(0)
{
	++m_count;

	cu::RefCountObjAssign(m_sym, sym);

	InitFlags();
}

Sprite::~Sprite()
{
	--m_count;

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

	SetDirty(true);
}

void Sprite::SetCenter(const sm::vec2& pos)
{
	SetPosition(pos - GetCenter() + GetPosition());
}

void Sprite::SetPosition(const sm::vec2& pos)
{
	if (m_geo->GetPosition() == pos) {
		return;
	}
	if (m_geo == SprDefault::Instance()->Geo()) {
		m_geo = new SprGeo;
	}

	m_geo->SetPosition(pos);

// 	// immediately
// 	m_bounding->SetTransform(m_position, m_offset, m_angle);
	// lazy
	SetBoundingDirty(true);

	SetDirty(true);
	SetWorldDirty(true);
}

void Sprite::SetAngle(float angle)
{
	if (m_geo->GetAngle() == angle) {
		return;
	}
	if (m_geo == SprDefault::Instance()->Geo()) {
		m_geo = new SprGeo;
	}

	m_geo->SetAngle(angle);

// 	// immediately
// 	m_bounding->SetTransform(m_position, m_offset, m_angle);
	// lazy
	SetBoundingDirty(true);

	SetDirty(true);
	SetWorldDirty(true);
}

void Sprite::SetScale(const sm::vec2& scale)
{
	if (m_geo->GetScale() == scale) {
		return;
	}
	if (m_geo == SprDefault::Instance()->Geo()) {
		m_geo = new SprGeo;
	}

	const sm::vec2& old_scale = m_geo->GetScale();
	if (old_scale.x != 0 && old_scale.y != 0) 
	{
		sm::vec2 dscale;
		dscale.x = scale.x / old_scale.x;
		dscale.y = scale.y / old_scale.y;

		if (!m_geo->GetOffset().IsValid()) {
			m_geo->SetOffset(m_sym->GetBounding(this).Center());
		}
		sm::vec2 old_offset = m_geo->GetOffset();
		sm::vec2 new_offset(old_offset.x * dscale.x, old_offset.y * dscale.y);
		m_geo->SetOffset(new_offset);

		m_geo->SetPosition(m_geo->GetPosition() + old_offset - new_offset);
	}

	m_geo->SetScale(scale);

	// lazy
	SetBoundingDirty(true);

	SetDirty(true);
	SetWorldDirty(true);
}

void Sprite::SetShear(const sm::vec2& shear)
{
	if (m_geo->GetShear() == shear) {
		return;
	}
	if (m_geo == SprDefault::Instance()->Geo()) {
		m_geo = new SprGeo;
	}

	sm::mat4 mat_old, mat_new;
	mat_old.Shear(m_geo->GetShear().x, m_geo->GetShear().y);
	mat_new.Shear(shear.x, shear.y);

	if (!m_geo->GetOffset().IsValid()) {
		m_geo->SetOffset(m_sym->GetBounding(this).Center());
	}
	sm::vec2 offset = mat_new * m_geo->GetOffset() - mat_old * m_geo->GetOffset();
	m_geo->SetOffset(m_geo->GetOffset() + offset);
	m_geo->SetPosition(m_geo->GetPosition() - offset);

	m_geo->SetShear(shear);

	// immediately
	m_bounding->SetTransform(m_geo->GetPosition(), m_geo->GetOffset(), m_geo->GetAngle());

	// 	// lazy
	// 	SetBoundingDirty(true); 

	SetDirty(true);
	SetWorldDirty(true);
}

void Sprite::SetOffset(const sm::vec2& offset)
{
	if (m_geo->GetOffset() == offset) {
		return;
	}
	if (m_geo == SprDefault::Instance()->Geo()) {
		m_geo = new SprGeo;
	}

	if (!m_geo->GetOffset().IsValid()) {
		m_geo->SetOffset(m_sym->GetBounding(this).Center());
	}

	// rotate + offset -> offset + rotate	
	sm::vec2 old_center = GetCenter();
	m_geo->SetOffset(offset);
	sm::vec2 new_center = GetCenter();
	m_geo->SetPosition(m_geo->GetPosition() + old_center - new_center);

	// immediately
	m_bounding->SetTransform(m_geo->GetPosition(), m_geo->GetOffset(), m_geo->GetAngle());

	// 	// lazy
	// 	SetBoundingDirty(true); 

	SetDirty(true);
	SetWorldDirty(true);
}

bool Sprite::Traverse(SprVisitor& visitor) const
{
	VisitResult ret = visitor.Visit(this);
	if (ret == VISIT_INTO) {
		return TraverseChildren(visitor);
	} else {
		return ret == VISIT_CONTINUE;
	}
}

const BoundingBox* Sprite::GetBounding() const 
{ 
	if (IsBoundingDirty()) {
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
	
	if (m_geo != SprDefault::Instance()->Geo() && !m_geo->GetOffset().IsValid()) {
		m_geo->SetOffset(rect.Center());
	}
	m_bounding->Build(rect, m_geo->GetPosition(), m_geo->GetAngle(), m_geo->GetScale(), 
		m_geo->GetShear(), m_geo->GetOffset());

	SetBoundingDirty(false);
}

void Sprite::Translate(const sm::vec2& trans) 
{ 
	SetPosition(m_geo->GetPosition() + trans);
}

void Sprite::Rotate(float rot) 
{ 
	SetAngle(m_geo->GetAngle() + rot); 
}

void Sprite::Scale(const sm::vec2& scale) 
{ 
	SetScale(m_geo->GetScale() * scale); 
}

sm::vec2 Sprite::GetCenter() const
{
	if (m_geo == SprDefault::Instance()->Geo()) {
		return sm::vec2(0, 0);
	} 

	if (!m_geo->GetOffset().IsValid()) {
		m_geo->SetOffset(m_sym->GetBounding(this).Center());
	}
	sm::vec2 center_offset = sm::rotate_vector(-m_geo->GetOffset(), m_geo->GetAngle()) + m_geo->GetOffset();
	sm::vec2 center = m_geo->GetPosition() + center_offset;
	return center;
}

const sm::vec2& Sprite::GetPosition() const	
{ 
	return m_geo->GetPosition();
}

float Sprite::GetAngle() const
{ 
	return m_geo->GetAngle();
}

const sm::vec2&	Sprite::GetScale() const
{
	return m_geo->GetScale();
}

const sm::vec2&	Sprite::GetShear() const
{
	return m_geo->GetShear();
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
	if (!m_geo->GetOffset().IsValid()) {
		m_geo->SetOffset(m_sym->GetBounding(this).Center());
	}
	return m_geo->GetOffset();
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

	SetDirty(true);
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

	SetDirty(true);
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

	SetDirty(true);
}

bool Sprite::IsVisible() const 
{ 
	return m_flags & FLAG_VISIBLE;
}

void Sprite::SetVisible(bool visible) const
{ 
	if (visible) {
		m_flags |= FLAG_VISIBLE;
	} else {
		m_flags &= ~FLAG_VISIBLE;
	}
}

bool Sprite::IsEditable() const 
{ 
	return m_flags & FLAG_EDITABLE; 
}

void Sprite::SetEditable(bool editable) const
{ 
	if (editable) {
		m_flags |= FLAG_EDITABLE;
	} else {
		m_flags &= ~FLAG_EDITABLE;
	}
}

bool Sprite::IsDirty() const
{
	return m_flags & FLAG_DIRTY; 
}

sm::mat4 Sprite::GetLocalMat() const
{
	if (m_geo == SprDefault::Instance()->Geo()) {
		return sm::mat4();
	} 
	
	sm::vec2 center = GetCenter();
	sm::mat4 mt;
	mt.SetTransformation(center.x, center.y, m_geo->GetAngle(), m_geo->GetScale().x, 
		m_geo->GetScale().y, 0, 0, m_geo->GetShear().x, m_geo->GetShear().y);
	return mt;
}

sm::mat4 Sprite::GetLocalInvMat() const
{
	if (m_geo == SprDefault::Instance()->Geo()) {
		return sm::mat4();
	} 

	sm::mat4 mat;
	mat.RotateZ(-m_geo->GetAngle() * SM_RAD_TO_DEG);
	mat.Shear(-m_geo->GetShear().x, -m_geo->GetShear().y);
	mat.Translate(-m_geo->GetPosition().x/m_geo->GetScale().x, -m_geo->GetPosition().y/m_geo->GetScale().y, 0);
	mat.Scale(1/m_geo->GetScale().x, 1/m_geo->GetScale().y, 1);
	return mat;
}

void Sprite::SetWorldMat(const sm::mat4& mat) const
{
	if (m_geo == SprDefault::Instance()->Geo()) {
		return;
	} 
	
	m_geo->SetWorldMat(mat);
}

void Sprite::InitFlags()
{
	SetBoundingDirty(true);
	SetVisible(true);
	SetEditable(true);
	SetDirty(false);
}

void Sprite::InitFromSpr(const Sprite& spr)
{
	cu::RefCountObjAssign(m_sym, const_cast<Symbol*>(spr.m_sym));

	m_name = spr.m_name;

	if (m_geo != spr.m_geo) 
	{
		if (m_geo && m_geo != SprDefault::Instance()->Geo()) {
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

	if (m_render != spr.m_render) 
	{
		if (m_render && m_render != SprDefault::Instance()->Render()) {
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

	m_flags = spr.m_flags;
}

bool Sprite::IsBoundingDirty() const
{
	return m_flags & FLAG_BOUNDING_DIRTY;
}

void Sprite::SetBoundingDirty(bool dirty) const
{
	if (dirty) {
		m_flags |= FLAG_BOUNDING_DIRTY;
	} else {
		m_flags &= ~FLAG_BOUNDING_DIRTY;
	}
}

void Sprite::SetDirty(bool dirty) const
{
	if (dirty) {
		m_flags |= FLAG_DIRTY;
	} else {
		m_flags &= ~FLAG_DIRTY;
	}
}

void Sprite::SetWorldDirty(bool dirty) const
{
	if (dirty) {
		m_flags |= FLAG_WORLD_DIRTY;
	} else {
		m_flags &= ~FLAG_WORLD_DIRTY;
	}
}

}