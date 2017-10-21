#include "S2_Sprite.h"
#include "S2_Symbol.h"
#include "FilterFactory.h"
#include "RenderColor.h"
#include "RenderShader.h"
#include "RenderCamera.h"
#include "SpriteVisitor.h"
#include "S2_Actor.h"
#include "SymType.h"
#include "SprVisitorParams.h"
#include "sprite2/Utility.h"
#include "UpdateParams.h"

#include <rigging.h>

#include <assert.h>

namespace s2
{

static int NEXT_ID = 0;

static int ALL_SPR_COUNT = 0;

#ifdef S2_SPR_CACHE_LOCAL_MAT_SHARE
static const sm::vec2 POS0_PROXY = sm::vec2(0, 0);
static const sm::vec2 POS1_PROXY = sm::vec2(1, 1);
#endif // S2_SPR_CACHE_LOCAL_MAT_SHARE

static void (*INIT_FLAGS)(const SprPtr& spr);

Sprite::Sprite()
	: m_sym(nullptr)
	, m_name(-1)
	, m_geo(SprDefault::Instance()->Geo(), geo_deleter)
	, m_bounding(nullptr, BoundingBox::Deleter)
	, m_render(SprDefault::Instance()->Render(), render_deleter)
	, m_flags(0)
	, m_id(NEXT_ID++)
{
	++ALL_SPR_COUNT;

	InitFlags();
}

Sprite::Sprite(const Sprite& spr)
	: m_sym(nullptr)
	, m_name(-1)
	, m_geo(SprDefault::Instance()->Geo(), geo_deleter)
	, m_bounding(nullptr, BoundingBox::Deleter)
	, m_render(SprDefault::Instance()->Render(), render_deleter)
	, m_flags(spr.m_flags)
	, m_id(NEXT_ID++)
{
	++ALL_SPR_COUNT;

	InitFromSpr(spr);
}

Sprite& Sprite::operator = (const Sprite& spr)
{
	InitFromSpr(spr);
	return *this;
}

Sprite::Sprite(const SymPtr& sym, uint32_t id)
	: m_sym(sym)
	, m_name(-1)
	, m_geo(SprDefault::Instance()->Geo(), geo_deleter)
	, m_bounding(nullptr, BoundingBox::Deleter)
	, m_render(SprDefault::Instance()->Render(), render_deleter)
	, m_flags(0)
	, m_id(NEXT_ID++)
{
	++ALL_SPR_COUNT;

	InitFlags();
}

Sprite::~Sprite()
{
	--ALL_SPR_COUNT;

	if (m_actors) {
		mm::AllocHelper::Delete(m_actors);
	}
}

SprPtr Sprite::Clone() const
{
	auto ret = CloneImpl();
	if (ret) {
		ret->OnMessage(UpdateParams(), MSG_START);
	}
	return ret;
}

void Sprite::SetSymbol(const SymPtr& sym)
{
	m_sym = sym;

	UpdateBounding(nullptr);

	SetDirty(true);
}

void Sprite::SetCenter(const sm::vec2& pos)
{
	SetPosition(pos - m_geo->GetCenter() + GetPosition());
}

void Sprite::SetPosition(const sm::vec2& pos)
{
	if (m_geo->GetPosition() == pos) {
		return;
	}
	if (m_geo.get() == SprDefault::Instance()->Geo()) {
		m_geo.reset(static_cast<SprGeo*>(mm::AllocHelper::New<SprGeo>()));
	}

#ifdef S2_SPR_CACHE_LOCAL_MAT_SHARE
	assert(!IsGeoMatrix());
#endif // S2_SPR_CACHE_LOCAL_MAT_SHARE
	m_geo->SetPosition(pos);

// 	// immediately
// 	m_bounding->SetTransform(m_position, m_offset, m_angle);
	// lazy
	SetBoundingDirty(true);

	SetDirty(true);
}

void Sprite::SetAngle(float angle)
{
	if (m_geo->GetAngle() == angle) {
		return;
	}
	if (m_geo.get() == SprDefault::Instance()->Geo()) {
		m_geo.reset(static_cast<SprGeo*>(mm::AllocHelper::New<SprGeo>()));
	}

#ifdef S2_SPR_CACHE_LOCAL_MAT_SHARE
	assert(!IsGeoMatrix());
#endif // S2_SPR_CACHE_LOCAL_MAT_SHARE
	m_geo->SetAngle(angle);

// 	// immediately
// 	m_bounding->SetTransform(m_position, m_offset, m_angle);
	// lazy
	SetBoundingDirty(true);

	SetDirty(true);
}

void Sprite::SetScale(const sm::vec2& scale)
{
	if (m_geo->GetScale() == scale) {
		return;
	}
	if (m_geo.get() == SprDefault::Instance()->Geo()) {
		m_geo.reset(static_cast<SprGeo*>(mm::AllocHelper::New<SprGeo>()));
	}

#ifdef S2_SPR_CACHE_LOCAL_MAT_SHARE
	assert(!IsGeoMatrix());
#endif // S2_SPR_CACHE_LOCAL_MAT_SHARE

	const sm::vec2& old_scale = m_geo->GetScale();
	if (old_scale.x != 0 && old_scale.y != 0) 
	{
		sm::vec2 dscale;
		dscale.x = scale.x / old_scale.x;
		dscale.y = scale.y / old_scale.y;

		sm::vec2 old_offset = m_geo->GetOffset();
		sm::vec2 new_offset(old_offset.x * dscale.x, old_offset.y * dscale.y);
		m_geo->SetOffset(new_offset);
		m_geo->SetPosition(m_geo->GetPosition() + old_offset - new_offset);
	}

	m_geo->SetScale(scale);

	// lazy
	SetBoundingDirty(true);

	SetDirty(true);
}

void Sprite::SetShear(const sm::vec2& shear)
{
	if (m_geo->GetShear() == shear) {
		return;
	}
	if (m_geo.get() == SprDefault::Instance()->Geo()) {
		m_geo.reset(static_cast<SprGeo*>(mm::AllocHelper::New<SprGeo>()));
	}

#ifdef S2_SPR_CACHE_LOCAL_MAT_SHARE
	assert(!IsGeoMatrix());
#endif // S2_SPR_CACHE_LOCAL_MAT_SHARE

	S2_MAT mat_old, mat_new;
#ifdef S2_MATRIX_FIX
	// todo
#else
	mat_old.Shear(m_geo->GetShear().x, m_geo->GetShear().y);
	mat_new.Shear(shear.x, shear.y);
#endif // S2_MATRIX_FIX

	sm::vec2 off = m_geo->GetOffset();
	sm::vec2 offset = mat_new * off - mat_old * off;
	m_geo->SetOffset(off + offset);
	m_geo->SetPosition(m_geo->GetPosition() - offset);

	m_geo->SetShear(shear);

	// immediately
	if (!m_bounding) {
		CreateBounding();
	}
	m_bounding->SetTransform(m_geo->GetPosition(), m_geo->GetOffset(), m_geo->GetAngle());

	// 	// lazy
	// 	SetBoundingDirty(true); 

	SetDirty(true);
}

void Sprite::SetOffset(const sm::vec2& offset)
{
	if (m_geo->GetOffset() == offset) {
		return;
	}
	if (m_geo.get() == SprDefault::Instance()->Geo()) {
		m_geo.reset(static_cast<SprGeo*>(mm::AllocHelper::New<SprGeo>()));
	}

#ifdef S2_SPR_CACHE_LOCAL_MAT_SHARE
	assert(!IsGeoMatrix());
#endif // S2_SPR_CACHE_LOCAL_MAT_SHARE

	// rotate + offset -> offset + rotate	
	sm::vec2 old_center = m_geo->GetCenter();
	m_geo->SetOffset(offset);
	sm::vec2 new_center = m_geo->GetCenter();
	m_geo->SetPosition(m_geo->GetPosition() + old_center - new_center);

	// immediately
	if (!m_bounding) {
		CreateBounding();
	}
	m_bounding->SetTransform(m_geo->GetPosition(), m_geo->GetOffset(), m_geo->GetAngle());

	// 	// lazy
	// 	SetBoundingDirty(true); 

	SetDirty(true);
}

//void Sprite::InitHook(void (*init_flags)(const SprPtr& spr))
//{
//	INIT_FLAGS = init_flags;
//}

int Sprite::GetAllSprCount()
{
	return ALL_SPR_COUNT;
}

VisitResult Sprite::Traverse(SpriteVisitor& visitor, const SprVisitorParams& params, bool init_mat) const
{
	SprVisitorParams p;
	p.actor = params.actor;
	if (init_mat) {
		Utility::PrepareMat(params.mt, this, params.actor, p.mt);
	}

	VisitResult ret = VISIT_OVER;

	VisitResult v_ret = visitor.Visit(shared_from_this(), p);
	switch (v_ret)
	{
	case VISIT_INTO:
		{
			visitor.VisitChildrenBegin(shared_from_this(), p);
			VisitResult v = TraverseChildren(visitor, p);
			switch (v)
			{
			case VISIT_INTO:
				assert(0);
				break;
			case VISIT_OVER:
				ret = visitor.VisitChildrenEnd(shared_from_this(), p);
				break;
			case VISIT_OUT: case VISIT_STOP:
				ret = v;
				break;
			}
		}
		break;
	default:
		ret = v_ret;
		break;
	}

	return ret;
}

VisitResult Sprite::Traverse2(SpriteVisitor2& visitor, const SprVisitorParams2& params, bool init_mat) const
{
	SprVisitorParams2 p;
	p.actor = params.actor;
	if (init_mat) {
		Utility::PrepareMat(params.mt, this, params.actor.get(), p.mt);
	}

	VisitResult ret = VISIT_OVER;

	VisitResult v_ret = visitor.Visit(shared_from_this(), p);
	switch (v_ret)
	{
	case VISIT_INTO:
	{
		visitor.VisitChildrenBegin(shared_from_this(), p);
		VisitResult v = TraverseChildren2(visitor, p);
		switch (v)
		{
		case VISIT_INTO:
			assert(0);
			break;
		case VISIT_OVER:
			ret = visitor.VisitChildrenEnd(shared_from_this(), p);
			break;
		case VISIT_OUT: case VISIT_STOP:
			ret = v;
			break;
		}
	}
	break;
	default:
		ret = v_ret;
		break;
	}

	return ret;
}

const BoundingBox* Sprite::GetBounding(const Actor* actor) const
{ 
	if (IsBoundingDirty()) {
		UpdateBounding(actor);
	}
	if (!m_bounding) {
		CreateBounding();
	}
	return m_bounding.get(); 
}

// todo: m_sym->GetBounding too slow, should be cached
void Sprite::UpdateBounding(const Actor* actor) const
{
	if (!m_sym) {
		return;
	}

	sm::rect rect = m_sym->GetBounding(this, actor);
	if (!rect.IsValid()) {
		return;
	}

#ifdef S2_SPR_CACHE_LOCAL_MAT_SHARE
	assert(!IsGeoMatrix());
#endif // S2_SPR_CACHE_LOCAL_MAT_SHARE

	if (!m_bounding) {
		CreateBounding();
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

const sm::vec2& Sprite::GetCenter() const
{
#ifdef S2_SPR_CACHE_LOCAL_MAT_SHARE
	return IsGeoMatrix() ? POS0_PROXY : m_geo->GetCenter();
#else
	return m_geo->GetCenter();
#endif // S2_SPR_CACHE_LOCAL_MAT_SHARE
}

const sm::vec2& Sprite::GetPosition() const	
{
#ifdef S2_SPR_CACHE_LOCAL_MAT_SHARE
	return IsGeoMatrix() ? POS0_PROXY : m_geo->GetPosition();
#else
	return m_geo->GetPosition();
#endif // S2_SPR_CACHE_LOCAL_MAT_SHARE
}

float Sprite::GetAngle() const
{
#ifdef S2_SPR_CACHE_LOCAL_MAT_SHARE
	return IsGeoMatrix() ? 0 : m_geo->GetAngle();
#else
	return m_geo->GetAngle();
#endif // S2_SPR_CACHE_LOCAL_MAT_SHARE
}

const sm::vec2& Sprite::GetScale() const
{
#ifdef S2_SPR_CACHE_LOCAL_MAT_SHARE
	return IsGeoMatrix() ? POS1_PROXY : m_geo->GetScale();
#else
	return m_geo->GetScale();
#endif // S2_SPR_CACHE_LOCAL_MAT_SHARE
}

const sm::vec2& Sprite::GetShear() const
{
#ifdef S2_SPR_CACHE_LOCAL_MAT_SHARE
	return IsGeoMatrix() ? POS0_PROXY : m_geo->GetShear();
#else
	return m_geo->GetShear();
#endif // S2_SPR_CACHE_LOCAL_MAT_SHARE
}

const sm::vec2& Sprite::GetOffset() const
{ 
#ifdef S2_SPR_CACHE_LOCAL_MAT_SHARE
	return IsGeoMatrix() ? POS0_PROXY : m_geo->GetOffset();
#else
	return m_geo->GetOffset();
#endif // S2_SPR_CACHE_LOCAL_MAT_SHARE
}

void Sprite::SetColor(const RenderColor& color)
{
	if (m_render.get() == SprDefault::Instance()->Render() || !m_render) {
		m_render.reset(static_cast<SprRender*>(mm::AllocHelper::New<SprRender>()));
	}
	m_render->SetColor(color);
	SetDirty(true);
}

void Sprite::SetShader(const RenderShader& shader)
{
	if (m_render.get() == SprDefault::Instance()->Render() || !m_render) {
		m_render.reset(static_cast<SprRender*>(mm::AllocHelper::New<SprRender>()));
	}
	m_render->SetShader(shader);
	SetDirty(true);
}

void Sprite::SetCamera(const RenderCamera& camera)
{
	if (m_render.get() == SprDefault::Instance()->Render() || !m_render) {
		m_render.reset(static_cast<SprRender*>(mm::AllocHelper::New<SprRender>()));
	}
	m_render->SetCamera(camera);
	SetDirty(true);
}

void Sprite::GetLocalSRT(SprSRT& srt) const
{
	m_geo->GetSRT(srt);
}

void Sprite::SetLocalSRT(const SprSRT& srt)
{
	if (m_geo.get() == SprDefault::Instance()->Geo()) {
		m_geo.reset(static_cast<SprGeo*>(mm::AllocHelper::New<SprGeo>()));
	}
	m_geo->SetSRT(srt);

	// lazy
	SetBoundingDirty(true);

	SetDirty(true);
}

//S2_MAT Sprite::GetLocalMat() const
//{
//	if (m_geo == SprDefault::Instance()->Geo()) {
//		return S2_MAT();
//	}
//
//#ifdef S2_SPR_CACHE_LOCAL_MAT_SHARE
//	if (IsGeoMatrix()) {
//		// todo
//		assert(!IsGeoDirty());
//		SetGeoDirty(false);
//		return m_geo->GetMatrix();
//	} else {
//		S2_MAT mat;
//		sm::vec2 center = GetCenter();
//		mat.SetTransformation(center.x, center.y, m_geo->GetAngle(), m_geo->GetScale().x, 
//			m_geo->GetScale().y, 0, 0, m_geo->GetShear().x, m_geo->GetShear().y);
//		SetGeoDirty(false);
//		return mat;
//	}
//#elif defined S2_SPR_CACHE_LOCAL_MAT_COPY
//	if (IsGeoDirty()) {
//		S2_MAT mat;
//		sm::vec2 center = GetCenter();
//		mat.SetTransformation(center.x, center.y, m_geo->GetAngle(), m_geo->GetScale().x, 
//			m_geo->GetScale().y, 0, 0, m_geo->GetShear().x, m_geo->GetShear().y);
//		m_geo->SetMatrix(mat);
//		SetGeoDirty(false);
//		return mat;
//	} else {
//		return m_geo->GetMatrix();
//	}
//#else
//	S2_MAT mat;
//	sm::vec2 center = GetCenter();
//	mat.SetTransformation(center.x, center.y, m_geo->GetAngle(), m_geo->GetScale().x, 
//		m_geo->GetScale().y, 0, 0, m_geo->GetShear().x, m_geo->GetShear().y);
//	return mat;
//#endif // S2_SPR_CACHE_LOCAL_MAT_SHARE
//}
//
//S2_MAT Sprite::GetLocalInvMat() const
//{
//	if (m_geo == SprDefault::Instance()->Geo()) {
//		return S2_MAT();
//	} 
//
//#ifdef S2_SPR_CACHE_LOCAL_MAT_SHARE
//	if (IsGeoMatrix()) {
//		return m_geo->GetMatrix().Inverted();
//	} else {
//#endif // S2_SPR_CACHE_LOCAL_MAT_SHARE
//		S2_MAT mat;
//		mat.Rotate(-m_geo->GetAngle());
//		mat.Translate(-m_geo->GetPosition().x/m_geo->GetScale().x, -m_geo->GetPosition().y/m_geo->GetScale().y);
//		mat.Scale(1/m_geo->GetScale().x, 1/m_geo->GetScale().y);
//		return mat;
//#ifdef S2_SPR_CACHE_LOCAL_MAT_SHARE
//	}
//#endif // S2_SPR_CACHE_LOCAL_MAT_SHARE
//}

const S2_MAT& Sprite::GetLocalMat() const
{
	return m_geo->GetMatrix();
}

#ifdef S2_SPR_CACHE_LOCAL_MAT_SHARE
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
#endif // S2_SPR_CACHE_LOCAL_MAT_SHARE

void Sprite::AddActor(const ActorPtr& actor) const
{
	if (!m_actors) {
		m_actors = mm::AllocHelper::New<SprActors>();
	}
	m_actors->Add(actor);
}

void Sprite::DelActor(const ActorPtr& actor) const
{
	if (m_actors) {
		m_actors->Del(actor);
	}
}

void Sprite::ClearActors() const
{
	if (m_actors) {
		m_actors->Clear();
	}
}

void Sprite::ConnectActors(const ActorPtr& parent) const
{
	if (m_actors) {
		m_actors->Connect(parent);
	}
}

void Sprite::InitFlags()
{
	m_flags = 0;

	SetVisible(true);
#ifdef S2_SPR_DEFAULT_EDITABLE
	SetEditable(S2_SPR_DEFAULT_EDITABLE);
#else
	SetEditable(true);
#endif // S2_SPR_DEFAULT_EDITABLE
	SetBoundingDirty(true);
	SetInheritUpdate(true);

	//if (INIT_FLAGS) {
	//	INIT_FLAGS(shared_from_this());
	//}
}

void Sprite::InitFromSpr(const Sprite& spr)
{
	m_sym = spr.m_sym;

	m_name = spr.m_name;

	if (m_geo != spr.m_geo) 
	{
 		if (spr.m_geo.get() == SprDefault::Instance()->Geo()) {
 			m_geo.reset(SprDefault::Instance()->Geo());
 		} else {
			if (m_geo.get() == SprDefault::Instance()->Geo()) {
				m_geo.reset(static_cast<SprGeo*>(mm::AllocHelper::New<SprGeo>()));
			}
 			*m_geo = *spr.m_geo;
 		}
	}

	if (spr.m_bounding) {
		if (!m_bounding) {
			CreateBounding();
		}
		*m_bounding = *spr.m_bounding;
	} else {
		m_bounding.reset();
	}

	if (m_render != spr.m_render) 
	{
		if (spr.m_render.get() == SprDefault::Instance()->Render()) 
		{
			m_render.reset(SprDefault::Instance()->Render());
		} 
		else 
		{
			if (m_render.get() == SprDefault::Instance()->Render()) {
				m_render.reset(static_cast<SprRender*>(mm::AllocHelper::New<SprRender>()));
			}

			auto& src_color = spr.m_render->GetColor();
			if (src_color && *src_color != *SprDefault::Instance()->Color()) {
				m_render->SetColor(*src_color);
			}

			auto& src_shader = spr.m_render->GetShader();
			if (src_shader && *src_shader != *SprDefault::Instance()->Shader()) {
				m_render->SetShader(*src_shader);
			}

			auto& src_camera = spr.m_render->GetCamera();
			if (src_camera && *src_camera != *SprDefault::Instance()->Camera()) {
				m_render->SetCamera(*src_camera);
			}
		}
	}

	m_flags = spr.m_flags;

	if (m_actors) {
		mm::AllocHelper::Delete(m_actors);
		m_actors = nullptr;
	}
}

bool Sprite::GetUserFlag(uint32_t key) const
{
	if (key < FLAG_MAX) {
		return false;
	}

	return (m_flags & key) != 0;
}

void Sprite::SetUserFlag(uint32_t key, bool val) const
{
	if (key < FLAG_MAX) {
		return;
	}

	if (val) {
		m_flags |= key;
	} else {
		m_flags &= ~key;
	}
}

}