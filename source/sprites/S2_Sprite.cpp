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
#include "S2_Actor.h"
#include "ActorLUT.h"
#include "ClearActorsVisitor.h"
#include "SymType.h"
#include "SprVisitorParams.h"

#include <assert.h>

namespace s2
{

int Sprite::m_count = 0;

#ifdef S2_SPR_CACHE_LOCAL_MAT
static const sm::vec2 POS0_PROXY = sm::vec2(0, 0);
static const sm::vec2 POS1_PROXY = sm::vec2(1, 1);
#endif // S2_SPR_CACHE_LOCAL_MAT

Sprite::Sprite()
	: m_sym(NULL)
	, m_geo(SprDefault::Instance()->Geo())
	, m_bounding(new OBB())
	, m_render(SprDefault::Instance()->Render())
	, m_flags(0)
{
	sm::vec2 scale = m_geo->GetScale();

	m_id = m_count;
	++m_count;

	InitFlags();
}

Sprite::Sprite(const Sprite& spr)
	: m_sym(NULL)
	, m_geo(SprDefault::Instance()->Geo())
	, m_bounding(NULL)
	, m_render(SprDefault::Instance()->Render())
	, m_flags(spr.m_flags)
{
	m_id = m_count;
	++m_count;

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
	, m_render(SprDefault::Instance()->Render())
	, m_flags(0)
{
	sm::vec2 scale = m_geo->GetScale();

	m_id = m_count;
	++m_count;

	cu::RefCountObjAssign(m_sym, sym);

	InitFlags();
}

Sprite::~Sprite()
{
	--m_count;

	ClearActorsVisitor visitor;
	Traverse(visitor, SprVisitorParams());

	if (m_sym) {
		m_sym->RemoveReference();
	}

	if (m_geo && m_geo != SprDefault::Instance()->Geo()) {
		SprGeoPool::Instance()->Push(m_geo);
	}

	delete m_bounding;

	if (m_render != SprDefault::Instance()->Render()) {
		SprRenderPool::Instance()->Push(m_render);
	}
}

void Sprite::AddReference() const
{
	cu::RefCountObj::AddReference();
}

void Sprite::RemoveReference() const
{
	cu::RefCountObj::RemoveReference();
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
		m_geo = SprGeoPool::Instance()->Pop();
	}

#ifdef S2_SPR_CACHE_LOCAL_MAT
	assert(!IsGeoMatrix());
#endif // S2_SPR_CACHE_LOCAL_MAT
	m_geo->SetPosition(pos);

// 	// immediately
// 	m_bounding->SetTransform(m_position, m_offset, m_angle);
	// lazy
	SetBoundingDirty(true);

	SetDirty(true);
	SetGeoDirty(true);
}

void Sprite::SetAngle(float angle)
{
	if (m_geo->GetAngle() == angle) {
		return;
	}
	if (m_geo == SprDefault::Instance()->Geo()) {
		m_geo = SprGeoPool::Instance()->Pop();
	}

#ifdef S2_SPR_CACHE_LOCAL_MAT
	assert(!IsGeoMatrix());
#endif // S2_SPR_CACHE_LOCAL_MAT
	m_geo->SetAngle(angle);

// 	// immediately
// 	m_bounding->SetTransform(m_position, m_offset, m_angle);
	// lazy
	SetBoundingDirty(true);

	SetDirty(true);
	SetGeoDirty(true);
}

void Sprite::SetScale(const sm::vec2& scale)
{
	if (m_geo->GetScale() == scale) {
		return;
	}
	if (m_geo == SprDefault::Instance()->Geo()) {
		m_geo = SprGeoPool::Instance()->Pop();
	}

#ifdef S2_SPR_CACHE_LOCAL_MAT
	assert(!IsGeoMatrix());
#endif // S2_SPR_CACHE_LOCAL_MAT

	const sm::vec2& old_scale = m_geo->GetScale();
	if (old_scale.x != 0 && old_scale.y != 0) 
	{
		sm::vec2 dscale;
		dscale.x = scale.x / old_scale.x;
		dscale.y = scale.y / old_scale.y;

		CheckInitOffset();
		sm::vec2 old_offset = m_geo->GetOffset();
		sm::vec2 new_offset(old_offset.x * dscale.x, old_offset.y * dscale.y);
		m_geo->SetOffset(new_offset);

		m_geo->SetPosition(m_geo->GetPosition() + old_offset - new_offset);
	}

	m_geo->SetScale(scale);

	// lazy
	SetBoundingDirty(true);

	SetDirty(true);
	SetGeoDirty(true);
}

void Sprite::SetShear(const sm::vec2& shear)
{
	if (m_geo->GetShear() == shear) {
		return;
	}
	if (m_geo == SprDefault::Instance()->Geo()) {
		m_geo = SprGeoPool::Instance()->Pop();
	}

#ifdef S2_SPR_CACHE_LOCAL_MAT
	assert(!IsGeoMatrix());
#endif // S2_SPR_CACHE_LOCAL_MAT

	S2_MAT mat_old, mat_new;
#ifdef S2_MATRIX_FIX
	// todo
#else
	mat_old.Shear(m_geo->GetShear().x, m_geo->GetShear().y);
	mat_new.Shear(shear.x, shear.y);
#endif // S2_MATRIX_FIX

	CheckInitOffset();
	sm::vec2 offset = mat_new * m_geo->GetOffset() - mat_old * m_geo->GetOffset();
	m_geo->SetOffset(m_geo->GetOffset() + offset);
	m_geo->SetPosition(m_geo->GetPosition() - offset);

	m_geo->SetShear(shear);

	// immediately
	m_bounding->SetTransform(m_geo->GetPosition(), m_geo->GetOffset(), m_geo->GetAngle());

	// 	// lazy
	// 	SetBoundingDirty(true); 

	SetDirty(true);
	SetGeoDirty(true);
}

void Sprite::SetOffset(const sm::vec2& offset)
{
	if (m_geo->GetOffset() == offset) {
		return;
	}
	if (m_geo == SprDefault::Instance()->Geo()) {
		m_geo = SprGeoPool::Instance()->Pop();
	}

#ifdef S2_SPR_CACHE_LOCAL_MAT
	assert(!IsGeoMatrix());
#endif // S2_SPR_CACHE_LOCAL_MAT

	CheckInitOffset();

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
	SetGeoDirty(true);
}

bool Sprite::Traverse(SprVisitor& visitor, const SprVisitorParams& params) const
{
	SprVisitorParams p;

	p.mt = GetLocalMat() * params.mt;
	p.path = params.path;
	p.path.Push(m_id);
	const Actor* actor = QueryActor(p.path);
	if (actor) {
		p.mt = actor->GetLocalMat() * p.mt;
	}

	VisitResult v_ret = visitor.Visit(this, p);
	if (v_ret == VISIT_INTO) 
	{
		visitor.VisitChildrenBegin(this, p);
		bool ret = TraverseChildren(visitor, p);
		visitor.VisitChildrenEnd(this, p);
		return ret;
	} 
	else 
	{
		return v_ret == VISIT_CONTINUE;
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
	if (!rect.IsValid()) {
		return;
	}

#ifdef S2_SPR_CACHE_LOCAL_MAT
	assert(!IsGeoMatrix());
#endif // S2_SPR_CACHE_LOCAL_MAT

	CheckInitOffset();
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

#ifdef S2_SPR_CACHE_LOCAL_MAT
	assert(!IsGeoMatrix());
#endif // S2_SPR_CACHE_LOCAL_MAT

	CheckInitOffset();
	sm::vec2 center_offset = sm::rotate_vector(-m_geo->GetOffset(), m_geo->GetAngle()) + m_geo->GetOffset();
	sm::vec2 center = m_geo->GetPosition() + center_offset;
	return center;
}

const sm::vec2& Sprite::GetPosition() const	
{
#ifdef S2_SPR_CACHE_LOCAL_MAT
	return IsGeoMatrix() ? POS0_PROXY : m_geo->GetPosition();
#else
	return m_geo->GetPosition();
#endif // S2_SPR_CACHE_LOCAL_MAT
}

float Sprite::GetAngle() const
{
#ifdef S2_SPR_CACHE_LOCAL_MAT
	return IsGeoMatrix() ? 0 : m_geo->GetAngle();
#else
	return m_geo->GetAngle();
#endif // S2_SPR_CACHE_LOCAL_MAT
}

const sm::vec2&	Sprite::GetScale() const
{
#ifdef S2_SPR_CACHE_LOCAL_MAT
	return IsGeoMatrix() ? POS1_PROXY : m_geo->GetScale();
#else
	return m_geo->GetScale();
#endif // S2_SPR_CACHE_LOCAL_MAT
}

const sm::vec2&	Sprite::GetShear() const
{
#ifdef S2_SPR_CACHE_LOCAL_MAT
	return IsGeoMatrix() ? POS0_PROXY : m_geo->GetShear();
#else
	return m_geo->GetShear();
#endif // S2_SPR_CACHE_LOCAL_MAT
}

const sm::vec2& Sprite::GetOffset() const
{ 
#ifdef S2_SPR_CACHE_LOCAL_MAT
	if (IsGeoMatrix()) {
		return POS0_PROXY;
	}
#endif // S2_SPR_CACHE_LOCAL_MAT

	if (m_geo == SprDefault::Instance()->Geo()) {
		m_geo = SprGeoPool::Instance()->Pop();
	}
	CheckInitOffset();
	return m_geo->GetOffset();
}

const RenderColor& Sprite::GetColor() const
{
	if (!m_render || (m_render && !m_render->GetColor())) {
		return *SprDefault::Instance()->Render()->GetColor();
	} else {
		return *m_render->GetColor();
	}
}

const RenderShader& Sprite::GetShader() const
{
	if (!m_render || (m_render && !m_render->GetShader())) {
		return *SprDefault::Instance()->Render()->GetShader();
	} else {
		return *m_render->GetShader();
	}
}

const RenderCamera& Sprite::GetCamera() const
{
	if (!m_render || (m_render && !m_render->GetCamera())) {
		return *SprDefault::Instance()->Render()->GetCamera();
	} else {
		return *m_render->GetCamera();
	}
}

void Sprite::SetColor(const RenderColor& color)
{
	if (m_render == SprDefault::Instance()->Render() || !m_render) {
		m_render = SprRenderPool::Instance()->Pop();
	}
	m_render->SetColor(color);
	SetDirty(true);
}

void Sprite::SetShader(const RenderShader& shader)
{
	if (m_render == SprDefault::Instance()->Render() || !m_render) {
		m_render = SprRenderPool::Instance()->Pop();
	}
	m_render->SetShader(shader);
	SetDirty(true);
}

void Sprite::SetCamera(const RenderCamera& camera)
{
	if (m_render == SprDefault::Instance()->Render() || !m_render) {
		m_render = SprRenderPool::Instance()->Pop();
	}
	m_render->SetCamera(camera);
	SetDirty(true);
}

S2_MAT Sprite::GetLocalMat() const
{
	if (m_geo == SprDefault::Instance()->Geo()) {
		return S2_MAT();
	}

#ifdef S2_SPR_CACHE_LOCAL_MAT
	if (IsGeoMatrix()) {
		// todo
		assert(!IsGeoDirty());
		SetGeoDirty(false);
		return m_geo->GetMatrix();
	} else {
#endif // S2_SPR_CACHE_LOCAL_MAT
		S2_MAT mat;
		sm::vec2 center = GetCenter();
		mat.SetTransformation(center.x, center.y, m_geo->GetAngle(), m_geo->GetScale().x, 
			m_geo->GetScale().y, 0, 0, m_geo->GetShear().x, m_geo->GetShear().y);
		SetGeoDirty(false);
		return mat;
#ifdef S2_SPR_CACHE_LOCAL_MAT
	}
#endif // S2_SPR_CACHE_LOCAL_MAT
}

S2_MAT Sprite::GetLocalInvMat() const
{
	if (m_geo == SprDefault::Instance()->Geo()) {
		return S2_MAT();
	} 

#ifdef S2_SPR_CACHE_LOCAL_MAT
	if (IsGeoMatrix()) {
		return m_geo->GetMatrix().Inverted();
	} else {
#endif // S2_SPR_CACHE_LOCAL_MAT
		S2_MAT mat;
#ifdef S2_MATRIX_FIX
		mat.Rotate(-m_geo->GetAngle());
		mat.Translate(-m_geo->GetPosition().x/m_geo->GetScale().x, -m_geo->GetPosition().y/m_geo->GetScale().y);
		mat.Scale(1/m_geo->GetScale().x, 1/m_geo->GetScale().y);
#else
		mat.RotateZ(-m_geo->GetAngle() * SM_RAD_TO_DEG);
		mat.Shear(-m_geo->GetShear().x, -m_geo->GetShear().y);
		mat.Translate(-m_geo->GetPosition().x/m_geo->GetScale().x, -m_geo->GetPosition().y/m_geo->GetScale().y, 0);
		mat.Scale(1/m_geo->GetScale().x, 1/m_geo->GetScale().y, 1);
#endif // S2_MATRIX_FIX
		return mat;
#ifdef S2_SPR_CACHE_LOCAL_MAT
	}
#endif // S2_SPR_CACHE_LOCAL_MAT
}

#ifdef S2_SPR_CACHE_LOCAL_MAT
void Sprite::CacheLocalMat()
{
	if (m_geo == SprDefault::Instance()->Geo()) {
		return;
	}
	if (IsGeoMatrix()) {
		return;
	}

	UpdateBounding();

	S2_MAT mat = GetLocalMat();
	m_geo->SetMatrix(mat);

	SetGeoMatrix(true);
}
#endif // S2_SPR_CACHE_LOCAL_MAT

void Sprite::AddActor(Actor* actor) const
{
	m_actors.push_back(actor);
}

const Actor* Sprite::QueryActor(const SprTreePath& path) const
{
	for (int i = 0, n = m_actors.size(); i < n; ++i) {
		if (m_actors[i]->GetTreePath() == path) {
			return m_actors[i];
		}
	}
	return NULL;
}

void Sprite::ClearActors() const
{
	for (int i = 0, n = m_actors.size(); i < n; ++i)
	{
		ActorLUT::Instance()->Delete(m_actors[i]);
		delete m_actors[i];
	}
}

void Sprite::FixActorPath(const SprTreePath& path)
{
	if (m_actors.empty()) {
		return;
	}
	SprTreePath this_path(path);
	this_path.Push(m_id);
	for (int i = 0, n = m_actors.size(); i < n; ++i) 
	{
		Actor* actor = m_actors[i];
		ActorLUT::Instance()->Delete(actor);
		actor->SetTreePath(this_path);
		ActorLUT::Instance()->Insert(actor);		
	}
}

void Sprite::InitFlags()
{
	SetVisible(true);
#ifdef S2_SPR_DEFAULT_EDITABLE
	SetEditable(S2_SPR_DEFAULT_EDITABLE);
#else
	SetEditable(true);
#endif // S2_SPR_DEFAULT_EDITABLE
	SetDirty(false);
	SetBoundingDirty(true);
#ifdef S2_SPR_CACHE_LOCAL_MAT
	SetGeoMatrix(false);
#endif // S2_SPR_CACHE_LOCAL_MAT
}

void Sprite::InitFromSpr(const Sprite& spr)
{
	cu::RefCountObjAssign(m_sym, const_cast<Symbol*>(spr.m_sym));

	m_name = spr.m_name;

	if (m_geo != spr.m_geo) 
	{
 		if (m_geo && m_geo != SprDefault::Instance()->Geo()) {
 			SprGeoPool::Instance()->Push(m_geo);
			m_geo = NULL;
 		}
 		if (spr.m_geo == SprDefault::Instance()->Geo()) {
 			m_geo = SprDefault::Instance()->Geo();
 		} else {
 			m_geo = SprGeoPool::Instance()->Pop();
 			*m_geo = *spr.m_geo;
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
			SprRenderPool::Instance()->Push(m_render);
			m_render = NULL;
		}
		if (spr.m_render == SprDefault::Instance()->Render()) 
		{
			m_render = SprDefault::Instance()->Render();
		} 
		else 
		{
			m_render = SprRenderPool::Instance()->Pop();

			const RenderColor* src_color = spr.m_render->GetColor();
			if (src_color && src_color != SprDefault::Instance()->Color()) {
				m_render->SetColor(*src_color);
			}

			const RenderShader* src_shader = spr.m_render->GetShader();
			if (src_shader && src_shader != SprDefault::Instance()->Shader()) {
				m_render->SetShader(*src_shader);
			}

			const RenderCamera* src_camera = spr.m_render->GetCamera();
			if (src_camera && src_camera != SprDefault::Instance()->Camera()) {
				m_render->SetCamera(*src_camera);
			}
		}
	}

	m_flags = spr.m_flags;
}

void Sprite::CheckInitOffset() const
{
#ifdef S2_SPR_CACHE_LOCAL_MAT
	assert(!IsGeoMatrix());
#endif // S2_SPR_CACHE_LOCAL_MAT
	if (m_geo->GetOffset().IsValid()) {
		return;
	}
	if (m_sym->Type() == SYM_IMAGE) {
		m_geo->SetOffset(m_sym->GetBounding(this).Center());
	} else {
		m_geo->SetOffset(sm::vec2(0, 0));
	}
}

}