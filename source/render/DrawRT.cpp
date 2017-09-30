#include "DrawRT.h"
#include "S2_RenderTargetMgr.h"
#include "S2_RenderTarget.h"
#include "RenderCtxStack.h"
#include "RenderParams.h"
#include "DrawNode.h"
#include "S2_Symbol.h"
#include "Shape.h"

#include <SM_Rect.h>
#include <unirender/UR_RenderContext.h>
#include <shaderlab/ShaderMgr.h>
#include <gimg_typedef.h>
#include <gimg_export.h>

#include <string.h>

namespace s2
{

DrawRT::DrawRT()
{
	m_rt = RenderTargetMgr::Instance()->Fetch();
	m_rt_type = SRC_MGR;
}

DrawRT::DrawRT(int width, int height)
{
	m_rt = new RenderTarget(width, height);
	m_rt_type = SRC_NEW;
}

DrawRT::DrawRT(RenderTarget* rt)
{
	m_rt = rt;
	m_rt_type = SRC_OUT;
}

DrawRT::~DrawRT()
{
	switch (m_rt_type)
	{
	case SRC_NEW:
		delete m_rt;
		break;
	case SRC_MGR:
		RenderTargetMgr::Instance()->Return(m_rt);
		break;
	}
}

void DrawRT::Draw(const Sprite* spr, bool clear, int width, int height, float dx, float dy, float scale)
{
	if (width == -1) {
		width = m_rt->Width();
	}
	if (height == -1) {
		height = m_rt->Height();
	}

	m_rt->Bind();

	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
	mgr->SetShader(sl::SPRITE2);

	if (clear) {
		ur::RenderContext* rc = sl::ShaderMgr::Instance()->GetContext();
		rc->SetClearFlag(ur::MASKC);
		rc->Clear(0);
	}

	RenderCtxStack::Instance()->Push(RenderContext(
		static_cast<float>(width), static_cast<float>(height), width, height));

	RenderParams params;
	params.mt.Scale(scale, -scale);
	params.mt.Translate(-dx, dy);
	params.SetChangeShader(false);
	DrawNode::Draw(spr, params);

	// todo 连续画symbol，不批量的话会慢。需要加个参数控制。
	mgr->FlushShader();

	RenderCtxStack::Instance()->Pop();

	m_rt->Unbind();
}

void DrawRT::Draw(const Symbol* sym, bool whitebg, float scale)
{
	m_rt->Bind();

	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
	mgr->SetShader(sl::SPRITE2);

	ur::RenderContext* rc = sl::ShaderMgr::Instance()->GetContext();
	rc->SetClearFlag(ur::MASKC);
	if (whitebg) {
		rc->Clear(0xffffffff);
	} else {
		rc->Clear(0);
	}	

	sm::rect rect = sym->GetBounding();
	sm::vec2 sz = rect.Size();
	int w = static_cast<int>(sz.x * scale),
		h = static_cast<int>(sz.y * scale);

	RenderCtxStack::Instance()->Push(RenderContext(
		static_cast<float>(w), static_cast<float>(h), w, h));

	RenderParams params;
	sm::vec2 center = rect.Center();
	float dx = -center.x;
	float dy = center.y;
	params.mt.Translate(dx * scale, dy * scale);
	DrawNode::Draw(sym, params, sm::vec2(0, 0), 0.0f, sm::vec2(scale, -scale));

	// todo 连续画symbol，不批量的话会慢。需要加个参数控制。
	mgr->FlushShader();

	RenderCtxStack::Instance()->Pop();

	m_rt->Unbind();
}

void DrawRT::Draw(const Shape* shape, bool clear, int width, int height)
{
	if (width == -1) {
		width = m_rt->Width();
	}
	if (height == -1) {
		height = m_rt->Height();
	}

	m_rt->Bind();

	if (clear) {
		ur::RenderContext* rc = sl::ShaderMgr::Instance()->GetContext();
		rc->SetClearFlag(ur::MASKC);
		rc->Clear(0);
	}

	RenderCtxStack::Instance()->Push(RenderContext(
		static_cast<float>(width), static_cast<float>(height), width, height));

	RenderParams rp;
	rp.mt.Scale(1, -1);
	shape->Draw(rp);

	RenderCtxStack::Instance()->Pop();

	m_rt->Unbind();
}

uint8_t* DrawRT::StoreToMemory(int width, int height, int channels)
{
	if (channels != 3 && channels != 4) {
		return nullptr;
	}

	if (width == -1) {
		width = m_rt->Width();
	}
	if (height == -1) {
		height = m_rt->Height();
	}

	int sz = width * height * channels;
	uint8_t* pixels = new uint8_t[sz];
	if (!pixels) {
		return nullptr;
	}

	memset(pixels, 0, sz);

	ur::RenderContext* rc = sl::ShaderMgr::Instance()->GetContext();
	m_rt->Bind();
	rc->ReadPixels(pixels, channels, 0, 0, width, height);
	m_rt->Unbind();
	return pixels;
}

void DrawRT::StoreToFile(const std::string& filepath, int width, int height, int channels)
{
	if (channels != 3 && channels != 4) {
		return;
	}

	uint8_t* pixels = StoreToMemory(width, height, channels);
	if (!pixels) {
		return;
	}

	GIMG_PIXEL_FORMAT fmt = channels == 3 ? GPF_RGB : GPF_RGBA8;
	gimg_export(filepath.c_str(), pixels, width, height, fmt, false);

	delete[] pixels;
}

}