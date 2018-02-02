#include "sprite2/Sprite.h"
#include "sprite2/Symbol.h"
#include "sprite2/SpriteVisitor.h"
#include "sprite2/Actor.h"
#include "sprite2/SymType.h"
#include "sprite2/SprVisitorParams.h"
#include "sprite2/Utility.h"
#include "sprite2/UpdateParams.h"
#include "sprite2/CompTransform.h"
#include "sprite2/CompBoundingBox.h"
#include "sprite2/CompColorCommon.h"
#include "sprite2/CompColorMap.h"
#include "sprite2/CompCamera.h"
#include "sprite2/CompShader.h"

#include <rigging.h>
#include <painting2/GeoTransform.h>
#include <painting2/RenderCamera.h>

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
	: m_name(-1)
	, m_flags(0)
	, m_id(NEXT_ID++)
{
	++ALL_SPR_COUNT;

	InitFlags();
}

Sprite::Sprite(const Sprite& spr)
	: ComponentsMgr(spr)
	, m_sym(spr.m_sym)
	, m_name(spr.m_name)
	, m_flags(spr.m_flags)
	, m_id(NEXT_ID++)
{
	++ALL_SPR_COUNT;
}

Sprite& Sprite::operator = (const Sprite& spr)
{
	ComponentsMgr::operator = (spr);

	m_sym = spr.m_sym;

	m_name = spr.m_name;

	m_flags = spr.m_flags;

	return *this;
}

Sprite::Sprite(const SymPtr& sym, uint32_t id)
	: m_sym(sym)
	, m_name(-1)
	, m_flags(0)
	, m_id(NEXT_ID++)
{
	++ALL_SPR_COUNT;

	InitFlags();
}

Sprite::~Sprite()
{
	--ALL_SPR_COUNT;
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
	SetPosition(pos - GetTransform().GetCenter() + GetPosition());
}

void Sprite::SetPosition(const sm::vec2& pos)
{
	if (GetTransform().GetPosition() == pos) {
		return;
	}

	auto& ctrans = HasComponent<CompTransform>() ? GetComponent<CompTransform>() : AddComponent<CompTransform>();
	ctrans.GetTrans().SetPosition(pos);

#ifdef S2_SPR_CACHE_LOCAL_MAT_SHARE
	assert(!IsGeoMatrix());
#endif // S2_SPR_CACHE_LOCAL_MAT_SHARE

// 	// immediately
// 	m_bounding->SetTransform(m_position, m_offset, m_angle);
	// lazy
	SetBoundingDirty(true);

	SetDirty(true);
}

void Sprite::SetAngle(float angle)
{
	if (GetTransform().GetAngle() == angle) {
		return;
	}

	auto& ctrans = HasComponent<CompTransform>() ? GetComponent<CompTransform>() : AddComponent<CompTransform>();
	ctrans.GetTrans().SetAngle(angle);

#ifdef S2_SPR_CACHE_LOCAL_MAT_SHARE
	assert(!IsGeoMatrix());
#endif // S2_SPR_CACHE_LOCAL_MAT_SHARE

// 	// immediately
// 	m_bounding->SetTransform(m_position, m_offset, m_angle);
	// lazy
	SetBoundingDirty(true);

	SetDirty(true);
}

void Sprite::SetScale(const sm::vec2& scale)
{
	const sm::vec2& old_scale = GetTransform().GetScale();
	if (old_scale == scale) {
		return;
	}

	auto& ctrans = HasComponent<CompTransform>() ? GetComponent<CompTransform>() : AddComponent<CompTransform>();

#ifdef S2_SPR_CACHE_LOCAL_MAT_SHARE
	assert(!IsGeoMatrix());
#endif // S2_SPR_CACHE_LOCAL_MAT_SHARE

	if (old_scale.x != 0 && old_scale.y != 0) 
	{
		sm::vec2 dscale;
		dscale.x = scale.x / old_scale.x;
		dscale.y = scale.y / old_scale.y;

		pt2::GeoTransform& trans = ctrans.GetTrans();
		sm::vec2 old_offset = trans.GetOffset();
		sm::vec2 new_offset(old_offset.x * dscale.x, old_offset.y * dscale.y);
		trans.SetOffset(new_offset);
		trans.SetPosition(trans.GetPosition() + old_offset - new_offset);
	}

	ctrans.GetTrans().SetScale(scale);

	// lazy
	SetBoundingDirty(true);

	SetDirty(true);
}

void Sprite::SetShear(const sm::vec2& shear)
{
	const sm::vec2& old_shear = GetTransform().GetShear();
	if (old_shear == shear) {
		return;
	}

#ifdef S2_SPR_CACHE_LOCAL_MAT_SHARE
	assert(!IsGeoMatrix());
#endif // S2_SPR_CACHE_LOCAL_MAT_SHARE

	auto& ctrans = HasComponent<CompTransform>() ? GetComponent<CompTransform>() : AddComponent<CompTransform>();
	pt2::GeoTransform& trans = ctrans.GetTrans();

	S2_MAT mat_old, mat_new;
#ifdef S2_MATRIX_FIX
	// todo
#else
	mat_old.Shear(trans.GetShear().x, trans.GetShear().y);
	mat_new.Shear(shear.x, shear.y);
#endif // S2_MATRIX_FIX

	sm::vec2 off = trans.GetOffset();
	sm::vec2 offset = mat_new * off - mat_old * off;
	trans.SetOffset(off + offset);
	trans.SetPosition(trans.GetPosition() - offset);

	trans.SetShear(shear);

	// immediately
	auto& cbounding = HasComponent<CompBoundingBox>() ? GetComponent<CompBoundingBox>() : AddComponent<CompBoundingBox>();
	auto& bb = cbounding.GetBB();
	bb.SetTransform(trans.GetPosition(), trans.GetOffset(), trans.GetAngle());

	// 	// lazy
	// 	SetBoundingDirty(true); 

	SetDirty(true);
}

void Sprite::SetOffset(const sm::vec2& offset)
{
	const sm::vec2& old_offset = GetTransform().GetOffset();
	if (old_offset == offset) {
		return;
	}

	auto& ctrans = HasComponent<CompTransform>() ? GetComponent<CompTransform>() : AddComponent<CompTransform>();

#ifdef S2_SPR_CACHE_LOCAL_MAT_SHARE
	assert(!IsGeoMatrix());
#endif // S2_SPR_CACHE_LOCAL_MAT_SHARE

	pt2::GeoTransform& trans = ctrans.GetTrans();

	// rotate + offset -> offset + rotate	
	sm::vec2 old_center = trans.GetCenter();
	trans.SetOffset(offset);
	sm::vec2 new_center = trans.GetCenter();
	trans.SetPosition(trans.GetPosition() + old_center - new_center);

	// immediately
	auto& cbounding = HasComponent<CompBoundingBox>() ? GetComponent<CompBoundingBox>() : AddComponent<CompBoundingBox>();
	auto& bb = cbounding.GetBB();
	bb.SetTransform(trans.GetPosition(), trans.GetOffset(), trans.GetAngle());

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

const pt2::BoundingBox& Sprite::GetBounding(const Actor* actor) const
{ 
	if (IsBoundingDirty()) {
		UpdateBounding(actor);
	}
	auto& cbounding = HasComponent<CompBoundingBox>() ? GetComponent<CompBoundingBox>() : AddComponent<CompBoundingBox>();
	return cbounding.GetBB();
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

	auto& cbounding = HasComponent<CompBoundingBox>() ? GetComponent<CompBoundingBox>() : AddComponent<CompBoundingBox>();
	auto& bb = cbounding.GetBB();
	auto& ctrans = GetTransform();
	bb.Build(rect, ctrans.GetPosition(), ctrans.GetAngle(), ctrans.GetScale(), 
		ctrans.GetShear(), ctrans.GetOffset());

	SetBoundingDirty(false);
}

void Sprite::Translate(const sm::vec2& trans) 
{ 
	SetPosition(GetTransform().GetPosition() + trans);
}

void Sprite::Rotate(float rot) 
{ 
	SetAngle(GetTransform().GetAngle() + rot); 
}

void Sprite::Scale(const sm::vec2& scale) 
{ 
	SetScale(GetTransform().GetScale() * scale); 
}

const sm::vec2& Sprite::GetCenter() const
{
#ifdef S2_SPR_CACHE_LOCAL_MAT_SHARE
	return IsGeoMatrix() ? POS0_PROXY : GetTransform().GetCenter();
#else
	return GetTransform().GetCenter();
#endif // S2_SPR_CACHE_LOCAL_MAT_SHARE
}

const sm::vec2& Sprite::GetPosition() const	
{
#ifdef S2_SPR_CACHE_LOCAL_MAT_SHARE
	return IsGeoMatrix() ? POS0_PROXY : GetTransform().GetPosition();
#else
	return GetTransform().GetPosition();
#endif // S2_SPR_CACHE_LOCAL_MAT_SHARE
}

float Sprite::GetAngle() const
{
#ifdef S2_SPR_CACHE_LOCAL_MAT_SHARE
	return IsGeoMatrix() ? 0 : GetTransform().GetAngle();
#else
	return GetTransform().GetAngle();
#endif // S2_SPR_CACHE_LOCAL_MAT_SHARE
}

const sm::vec2& Sprite::GetScale() const
{
#ifdef S2_SPR_CACHE_LOCAL_MAT_SHARE
	return IsGeoMatrix() ? POS1_PROXY : GetTransform().GetScale();
#else
	return GetTransform().GetScale();
#endif // S2_SPR_CACHE_LOCAL_MAT_SHARE
}

const sm::vec2& Sprite::GetShear() const
{
#ifdef S2_SPR_CACHE_LOCAL_MAT_SHARE
	return IsGeoMatrix() ? POS0_PROXY : GetTransform().GetShear();
#else
	return GetTransform().GetShear();
#endif // S2_SPR_CACHE_LOCAL_MAT_SHARE
}

const sm::vec2& Sprite::GetOffset() const
{ 
#ifdef S2_SPR_CACHE_LOCAL_MAT_SHARE
	return IsGeoMatrix() ? POS0_PROXY : GetTransform().GetOffset();
#else
	return GetTransform().GetOffset();
#endif // S2_SPR_CACHE_LOCAL_MAT_SHARE
}

const pt2::RenderColorCommon& Sprite::GetColorCommon() const
{
	return HasComponent<CompColorCommon>() ?
		GetComponent<CompColorCommon>().GetColor() : SprDefault::Instance()->Color().GetColor();
}

const pt2::RenderColorMap& Sprite::GetColorMap() const
{
	return HasComponent<CompColorMap>() ?
		GetComponent<CompColorMap>().GetColor() : SprDefault::Instance()->ColorMap().GetColor();
}

const pt2::RenderCamera& Sprite::GetCamera() const 
{ 
	return HasComponent<CompCamera>() ?
		GetComponent<CompCamera>().GetCamera() : SprDefault::Instance()->Camera().GetCamera();
}

const pt2::RenderShader& Sprite::GetShader() const
{
	return HasComponent<CompShader>() ?
		GetComponent<CompShader>().GetShader() : SprDefault::Instance()->Shader().GetShader();
}

void Sprite::SetColorCommon(const pt2::RenderColorCommon& col)
{
	if (GetColorCommon() == col) {
		return;
	}

	auto& ccol = HasComponent<CompColorCommon>() ? GetComponent<CompColorCommon>() : AddComponent<CompColorCommon>();
	ccol.SetColor(col);
	SetDirty(true);
}

void Sprite::SetColorMap(const pt2::RenderColorMap& col)
{
	if (GetColorMap() == col) {
		return;
	}

	auto& ccol = HasComponent<CompColorMap>() ? GetComponent<CompColorMap>() : AddComponent<CompColorMap>();
	ccol.SetColor(col);
	SetDirty(true);
}

void Sprite::SetCamera(const pt2::RenderCamera& camera)
{
	if (GetCamera() == camera) {
		return;
	}
	auto& ccamera = HasComponent<CompCamera>() ? GetComponent<CompCamera>() : AddComponent<CompCamera>();
	ccamera.SetCamera(camera);
	SetDirty(true);
}

void Sprite::SetShader(const pt2::RenderShader& shader)
{
	if (GetShader() == shader) {
		return;
	}
	auto& cshader = HasComponent<CompShader>() ? GetComponent<CompShader>() : AddComponent<CompShader>();
	cshader.SetShader(shader);
	SetDirty(true);
}

const pt2::SRT& Sprite::GetLocalSRT() const
{
	return GetTransformComp().GetSrt();
}

void Sprite::SetLocalSRT(const pt2::SRT& srt)
{
	const_cast<CompTransform&>(GetTransformComp()).SetSrt(srt);

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
	return GetTransform().GetMatrix();
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
	auto& cactors = HasComponent<CompActors>() ? GetComponent<CompActors>() : AddComponent<CompActors>();
	cactors.Actors().Add(actor);
}

void Sprite::DelActor(const ActorPtr& actor) const
{
	if (HasComponent<CompActors>()) {
		GetComponent<CompActors>().Actors().Del(actor);
	}
}

void Sprite::ClearActors() const
{
	if (HasComponent<CompActors>()) {
		GetComponent<CompActors>().Actors().Clear();
	}
}

void Sprite::ConnectActors(const ActorPtr& parent) const
{
	if (HasComponent<CompActors>()) {
		GetComponent<CompActors>().Actors().Connect(parent);
	}
}

void Sprite::InitFlags()
{
	m_flags = 0;

	SetVisible(true);
	SetEditable(S2_SPR_DEFAULT_EDITABLE);
	SetBoundingDirty(true);
	SetInheritUpdate(true);

	//if (INIT_FLAGS) {
	//	INIT_FLAGS(shared_from_this());
	//}
}

const CompTransform& Sprite::GetTransformComp() const
{
	return HasComponent<CompTransform>() ?
		GetComponent<CompTransform>() : SprDefault::Instance()->Transform();
}

const pt2::GeoTransform& Sprite::GetTransform() const
{
	return GetTransformComp().GetTrans();
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