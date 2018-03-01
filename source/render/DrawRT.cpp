#include "sprite2/DrawRT.h"
#include "sprite2/RenderParams.h"
#include "sprite2/DrawNode.h"
#include "sprite2/Symbol.h"
#include "sprite2/Shape.h"

#include <SM_Rect.h>
#include <unirender/RenderContext.h>
#include <unirender/Blackboard.h>
#include <shaderlab/Blackboard.h>
#include <shaderlab/ShaderMgr.h>
#include <shaderlab/RenderContext.h>
#include <gimg_typedef.h>
#include <gimg_export.h>
#ifndef S2_DISABLE_DEFERRED
#include <cooking/Facade.h>
#endif // S2_DISABLE_DEFERRED
#include <painting2/RenderTargetMgr.h>
#include <painting2/RenderTarget.h>
#include <painting2/WndCtxStack.h>
#include <painting2/Blackboard.h>
#include <painting2/RenderContext.h>

#include <string.h>

namespace s2
{

DrawRT::DrawRT()
{
	m_rt = pt2::Blackboard::Instance()->GetContext().GetRTMgr().Fetch();
	m_rt_type = SRC_MGR;
}

DrawRT::DrawRT(int width, int height)
{
	m_rt = new pt2::RenderTarget(width, height);
	m_rt_type = SRC_NEW;
}

DrawRT::DrawRT(pt2::RenderTarget* rt)
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
		pt2::Blackboard::Instance()->GetContext().GetRTMgr().Return(m_rt);
		break;
	}
}

void DrawRT::Draw(const Sprite& spr, bool clear, int width, int height, float dx, float dy, float scale)
{
	if (width == -1) {
		width = m_rt->Width();
	}
	if (height == -1) {
		height = m_rt->Height();
	}

	m_rt->Bind();

	auto& rc = sl::Blackboard::Instance()->GetRenderContext();
	rc.GetShaderMgr().SetShader(sl::SPRITE2);

	if (clear) {
		auto& ur_rc = ur::Blackboard::Instance()->GetRenderContext();
		ur_rc.SetClearFlag(ur::MASKC);
		ur_rc.Clear(0);
	}

	auto& pt2_ctx = pt2::Blackboard::Instance()->GetContext();
	pt2_ctx.GetCtxStack().Push(pt2::WindowContext(
		static_cast<float>(width), static_cast<float>(height), width, height));

	RenderParams params;
	params.mt.Scale(scale, -scale);
	params.mt.Translate(-dx, dy);
	params.SetChangeShader(false);
	DrawNode::Draw(nullptr, &spr, params);

	// todo 连续画symbol，不批量的话会慢。需要加个参数控制。
	rc.GetShaderMgr().FlushShader();

	pt2_ctx.GetCtxStack().Pop();

	m_rt->Unbind();
}

void DrawRT::Draw(const Symbol& sym, bool whitebg, float scale)
{
	m_rt->Bind();

#ifdef S2_DISABLE_DEFERRED
	auto& shader_mgr = sl::Blackboard::Instance()->GetRenderContext().GetShaderMgr();
	shader_mgr.SetShader(sl::SPRITE2);

	auto& ur_rc = ur::Blackboard::Instance()->GetRenderContext();
	ur_rc.SetClearFlag(ur::MASKC);
#else
	cooking::change_shader(nullptr, sl::SPRITE2);
	cooking::set_render_clear_flag(nullptr, ur::MASKC);
#endif // S2_DISABLE_DEFERRED
	uint32_t clear_color;
	if (whitebg) {
		clear_color = 0xffffffff;
	} else {
		clear_color = 0;
	}
#ifdef S2_DISABLE_DEFERRED
	ur_rc.Clear(clear_color);
#else
	cooking::render_clear(nullptr, clear_color);
#endif // S2_DISABLE_DEFERRED

	sm::rect rect = sym.GetBounding();
	sm::vec2 sz = rect.Size();
	int w = static_cast<int>(sz.x * scale),
		h = static_cast<int>(sz.y * scale);

	auto& pt2_ctx = pt2::Blackboard::Instance()->GetContext();
	pt2_ctx.GetCtxStack().Push(pt2::WindowContext(
		static_cast<float>(w), static_cast<float>(h), w, h));

	RenderParams params;
	sm::vec2 center = rect.Center();
	float dx = -center.x;
	float dy = center.y;
	params.mt.Translate(dx * scale, dy * scale);
	DrawNode::Draw(sym, params, sm::vec2(0, 0), 0.0f, sm::vec2(scale, -scale));

	// todo 连续画symbol，不批量的话会慢。需要加个参数控制。
#ifdef S2_DISABLE_DEFERRED
	shader_mgr.FlushShader();
#else
	cooking::flush_shader(nullptr);
#endif // S2_DISABLE_DEFERRED

	pt2_ctx.GetCtxStack().Pop();

	m_rt->Unbind();
}

void DrawRT::Draw(const Shape& shape, bool clear, int width, int height)
{
	if (width == -1) {
		width = m_rt->Width();
	}
	if (height == -1) {
		height = m_rt->Height();
	}

	m_rt->Bind();

	if (clear) 
	{
#ifdef S2_DISABLE_DEFERRED
		auto& ur_rc = ur::Blackboard::Instance()->GetRenderContext();
		ur_rc.SetClearFlag(ur::MASKC);
		ur_rc.Clear(0);
#else
		cooking::set_render_clear_flag(nullptr, ur::MASKC);
		cooking::render_clear(nullptr, 0);
#endif // S2_DISABLE_DEFERRED
	}

	auto& pt2_ctx = pt2::Blackboard::Instance()->GetContext();
	pt2_ctx.GetCtxStack().Push(pt2::WindowContext(
		static_cast<float>(width), static_cast<float>(height), width, height));

	RenderParams rp;
	rp.mt.Scale(1, -1);
	shape.Draw(nullptr, rp);

	pt2_ctx.GetCtxStack().Pop();

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

	auto& ur_rc = ur::Blackboard::Instance()->GetRenderContext();
	m_rt->Bind();
	ur_rc.ReadPixels(pixels, channels, 0, 0, width, height);
	m_rt->Unbind();
	return pixels;
}

void DrawRT::StoreToFile(const CU_STR& filepath, int width, int height, int channels)
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