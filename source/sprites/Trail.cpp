#include "sprite2/Trail.h"
#include "sprite2/Symbol.h"
#include "sprite2/RenderParams.h"
#include "sprite2/DrawNode.h"

#include <cu/cu_stl.h>
#include <memmgr/Allocator.h>
#include <mt_2d.h>
#include <sm_const.h>
#include <unirender/RenderContext.h>
#include <shaderlab/Blackboard.h>
#include <shaderlab/ShaderMgr.h>
#include <shaderlab/RenderContext.h>
#include <shaderlab/Shape2Shader.h>

#include <assert.h>
#include <string.h>

namespace s2
{

CU_SINGLETON_DEFINITION(Trail)

Trail::Trail()
	: m_time(0)
{
	Init();
}

void Trail::Update(float dt)
{
	m_time += dt;
}

static void 
render_symbol_func(void* sym, float x, float y, float angle, float scale, uint8_t* mul_col, uint8_t* add_col, const void* ud)
{
	assert(ud);
	const TrailRenderParams* rp = (static_cast<const TrailRenderParams*>(ud));

	Symbol* s2_sym(static_cast<Symbol*>(sym));

	RenderParamsProxy rp_proxy;
	RenderParams* rp_child = rp_proxy.obj;
	rp_child->Reset();

	rp_child->mt = rp->mat;

	pt2::Color mul, add;
	memcpy(&mul.r, mul_col, sizeof(uint8_t) * 4);
	memcpy(&add.r, add_col, sizeof(uint8_t) * 4);
	rp_child->col_common.mul = mul * rp->col.mul;
	rp_child->col_common.add = add + rp->col.add;

	DrawNode::Draw(*s2_sym, *rp_child, sm::vec2(x, y), angle - SM_PI * 0.5f, sm::vec2(scale, scale));
}

static void 
render_shape_func(const float* positions, const uint32_t* colors, int count, const void* ud)
{
	assert(ud);
	const TrailRenderParams* rp = (static_cast<const TrailRenderParams*>(ud));

	CU_VEC<uint32_t> t_colors;
	t_colors.resize(count);
	for (int i = 0; i < count; ++i) {
		uint32_t col = colors[i];
		int a = (col >> 24) & 0xff,
			b = (col >> 16) & 0xff,
			g = (col >>  8) & 0xff,
			r = (col) & 0xff;
		auto& mul = rp->col.mul;
		auto& add = rp->col.add;
		a = static_cast<int>(a * mul.a / 255.0f);
		b = static_cast<int>(b * mul.b / 255.0f);
		g = static_cast<int>(g * mul.g / 255.0f);
		r = static_cast<int>(r * mul.r / 255.0f);
		a += add.a;
		b += add.b;
		g += add.g;
		r += add.r;
		t_colors[i] = (a << 24) | (b << 16) | (g << 8) | r;
	}

	auto& rc = sl::Blackboard::Instance()->GetRenderContext();
	rc.GetShaderMgr().SetShader(sl::SHAPE2);
	sl::Shape2Shader* shader = static_cast<sl::Shape2Shader*>(rc.GetShaderMgr().GetShader());

	auto& ur_rc = rc.GetContext();
	ur_rc.SetBlend(5, 6);		// BLEND_SRC_COLOR, BLEND_ONE_MINUS_SRC_ALPHA
	ur_rc.SetBlendEquation(0);	// BLEND_FUNC_ADD

	//	shader->Commit();

	shader->SetType(0x0005);	// todo from rvg_render.c

	CU_VEC<sm::vec2> vertices;
	vertices.resize(count);
	int ptr = 0;
	for (int i = 0; i < count; ++i) {
		float x = positions[ptr++],
		      y = positions[ptr++];		
		vertices[i] = rp->mat * sm::vec2(x, y);
	}
	shader->Draw(&vertices[0].x, &t_colors[0], count);
}

void Trail::Init()
{
	t2d_init();
	t2d_regist_cb(render_symbol_func, render_shape_func);	
}

}