#include "Trail.h"
#include "S2_Symbol.h"
#include "RenderParams.h"
#include "DrawNode.h"

#include <mt_2d.h>
#include <sm_const.h>
#include <unirender/RenderContext.h>
#include <shaderlab/ShaderMgr.h>
#include <shaderlab/Shape2Shader.h>

#include <vector>

#include <assert.h>

namespace s2
{

SINGLETON_DEFINITION(Trail)

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

	Symbol* s2_sym = static_cast<Symbol*>(sym);

	RenderParams params;

	params.mt = rp->mat;

	memcpy(&params.color.mul.r, mul_col, sizeof(uint8_t) * 4);
	memcpy(&params.color.add.r, add_col, sizeof(uint8_t) * 4);
	params.color.mul = params.color.mul * rp->ct.mul;
	params.color.add = params.color.add + rp->ct.add;

	DrawNode::Draw(s2_sym, params, sm::vec2(x, y), angle - SM_PI * 0.5f, sm::vec2(scale, scale));
}

static void 
render_shape_func(const float* positions, const uint32_t* colors, int count, const void* ud)
{
	assert(ud);
	const TrailRenderParams* rp = (static_cast<const TrailRenderParams*>(ud));

	std::vector<uint32_t> t_colors;
	t_colors.resize(count);
	for (int i = 0; i < count; ++i) {
		uint32_t col = colors[i];
		int a = (col >> 24) & 0xff,
			b = (col >> 16) & 0xff,
			g = (col >>  8) & 0xff,
			r = (col) & 0xff;
		a *= rp->ct.mul.a / 255.0f;
		b *= rp->ct.mul.b / 255.0f;
		g *= rp->ct.mul.g / 255.0f;
		r *= rp->ct.mul.r / 255.0f;
		a += rp->ct.add.a;
		b += rp->ct.add.b;
		g += rp->ct.add.g;
		r += rp->ct.add.r;
		t_colors[i] = (a << 24) | (b << 16) | (g << 8) | r;
	}

	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
	mgr->SetShader(sl::SHAPE2);
	sl::Shape2Shader* shader = static_cast<sl::Shape2Shader*>(mgr->GetShader());

	ur::RenderContext* rc = sl::ShaderMgr::Instance()->GetContext();
	rc->SetBlend(5, 6);		// BLEND_SRC_COLOR, BLEND_ONE_MINUS_SRC_ALPHA
	rc->SetBlendEquation(0);	// BLEND_FUNC_ADD

	//	shader->Commit();

	shader->SetType(0x0005);	// todo from rvg_render.c

	std::vector<sm::vec2> vertices;
	vertices.resize(count);
	int ptr = 0;
	for (int i = 0; i < count; ++i) {
		vertices[i] = rp->mat * sm::vec2(positions[ptr++], positions[ptr++]);
	}
	shader->Draw(&vertices[0].x, &t_colors[0], count);
}

void Trail::Init()
{
	t2d_init();
	t2d_regist_cb(render_symbol_func, render_shape_func);	
}

}