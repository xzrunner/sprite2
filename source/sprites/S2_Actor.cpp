#include "S2_Actor.h"
#include "S2_Sprite.h"
#include "ActorGeo.h"
#include "FixActorPathVisitor.h"
#include "SprVisitorParams.h"
#include "SprDefault.h"
#include "SprSRT.h"
#include "SprRender.h"

namespace s2
{

#ifdef S2_RES_LOG
static int COUNT = 0;
#endif // S2_RES_LOG

Actor::Actor()
	: m_spr(NULL)
	, m_geo(NULL)
	, m_render(SprDefault::Instance()->Render())
	, m_proxy(NULL)
{
	InitFlags();
}

Actor::Actor(const Sprite* spr, const SprTreePath& path)
	: m_spr(spr)
	, m_path(path)
	, m_geo(NULL)
	, m_render(SprDefault::Instance()->Render())
	, m_proxy(NULL)
{
#ifdef S2_RES_LOG
	++COUNT;
	std::cout << "++ actor " << COUNT << "\n";
#endif // S2_RES_LOG

	if (m_spr) {
		m_spr->AddActor(this);
	}

	InitFlags();
}

Actor::~Actor()
{
#ifdef S2_RES_LOG
	--COUNT;
	std::cout << "-- actor " << COUNT << "\n";
#endif // S2_RES_LOG

	if (m_geo) {
		delete m_geo;
	}
	if (m_render != SprDefault::Instance()->Render()) {
		SprRenderPool::Instance()->Push(m_render);
	}
	if (m_proxy) {
		m_proxy->RemoveReference();
	}
}

void Actor::SetPosition(const sm::vec2& pos)
{
	if (m_geo) {
		m_geo->SetPosition(pos);
	} else {
		if (pos != sm::vec2(0, 0)) {
			m_geo = new ActorGeo;
			m_geo->SetPosition(pos);
		}
	}
}

sm::vec2 Actor::GetPosition() const
{
	if (m_geo) {
		return m_geo->GetPosition();
	} else {
		return sm::vec2(0, 0);
	}
}

void Actor::SetAngle(float angle)
{
	if (m_geo) {
		m_geo->SetAngle(angle);
	} else {
		if (angle != 0) {
			m_geo = new ActorGeo;
			m_geo->SetAngle(angle);
		}
	}
}

float Actor::GetAngle() const
{
	if (m_geo) {
		return m_geo->GetAngle();
	} else {
		return 0;
	}
}

void Actor::SetScale(const sm::vec2& scale)
{
	if (m_geo) {
		m_geo->SetScale(scale);
	} else {
		if (scale != sm::vec2(1, 1)) {
			m_geo = new ActorGeo;
			m_geo->SetScale(scale);
		}
	}
}

sm::vec2 Actor::GetScale() const
{
	if (m_geo) {
		return m_geo->GetScale();
	} else {
		return sm::vec2(1, 1);
	}
}

S2_MAT Actor::GetLocalMat() const
{
	if (!m_geo) {
		return S2_MAT();
	}

	S2_MAT mt;
	mt.SetTransformation(m_geo->GetPosition().x, m_geo->GetPosition().y, m_geo->GetAngle(), 
		m_geo->GetScale().x, m_geo->GetScale().y, 0, 0, 0, 0);
	return mt;
}

const RenderColor& Actor::GetColor() const
{
	if (!m_render || (m_render && !m_render->GetColor())) {
		return *SprDefault::Instance()->Render()->GetColor();
	} else {
		return *m_render->GetColor();
	}
}

const RenderShader& Actor::GetShader() const
{
	if (!m_render || (m_render && !m_render->GetShader())) {
		return *SprDefault::Instance()->Render()->GetShader();
	} else {
		return *m_render->GetShader();
	}
}

const RenderCamera& Actor::GetCamera() const
{
	if (!m_render || (m_render && !m_render->GetCamera())) {
		return *SprDefault::Instance()->Render()->GetCamera();
	} else {
		return *m_render->GetCamera();
	}
}

void Actor::SetColor(const RenderColor& color)
{
	if (m_render == SprDefault::Instance()->Render() || !m_render) {
		m_render = SprRenderPool::Instance()->Pop();
	}
	m_render->SetColor(color);
}

void Actor::SetShader(const RenderShader& shader)
{
	if (m_render == SprDefault::Instance()->Render() || !m_render) {
		m_render = SprRenderPool::Instance()->Pop();
	}
	m_render->SetShader(shader);
}

void Actor::SetCamera(const RenderCamera& camera)
{
	if (m_render == SprDefault::Instance()->Render() || !m_render) {
		m_render = SprRenderPool::Instance()->Pop();
	}
	m_render->SetCamera(camera);
}

void Actor::SetProxy(Sprite* proxy)
{
	cu::RefCountObjAssign(m_proxy, proxy);
	if (!proxy) {
		return;
	}

	SprSRT srt;
	m_spr->GetLocalSRT(srt);
	m_proxy->SetLocalSRT(srt);

	m_spr->SetHasProxy(true);

	SprTreePath parent_path = m_path;
	parent_path.Pop();
	FixActorPathVisitor visitor(parent_path);
	m_proxy->Traverse(visitor, SprVisitorParams());
}

void Actor::InitFlags()
{
	SetVisible(true);
#ifdef S2_SPR_DEFAULT_EDITABLE
	SetEditable(S2_SPR_DEFAULT_EDITABLE);
#else
	SetEditable(true);
#endif // S2_SPR_DEFAULT_EDITABLE
}

}