#include "Scale9.h"
#include "S2_Sprite.h"
#include "S2_Symbol.h"
#include "DrawNode.h"
#include "ImageSymbol.h"
#include "S2_Texture.h"
#include "SymType.h"

#include <SM_Calc.h>

#include <string.h>
#include <assert.h>

namespace s2
{

Scale9::Scale9()
	: m_type(S9_NULL)
	, m_width(0)
	, m_height(0)
{
	m_sz_left = m_sz_right = m_sz_top = m_sz_down = 0;
}

Scale9::Scale9(const Scale9& s9)
{
	this->operator = (s9);
}

Scale9& Scale9::operator = (const Scale9& s9)
{
	m_type = s9.m_type;
	m_width = s9.m_width;
	m_height = s9.m_height;
	for (int i = 0; i < 9; ++i) 
	{
		const SprPtr& spr = s9.m_grids[i];
		if (spr) {
			m_grids[i] = spr->Clone();
		} else {
			m_grids[i] = nullptr;
		}
	}
	m_sz_left  = s9.m_sz_left;
	m_sz_right = s9.m_sz_right;
	m_sz_top   = s9.m_sz_top;
	m_sz_down  = s9.m_sz_down;
	return *this;
}

RenderReturn Scale9::Draw(const RenderParams& rp) const
{
	RenderReturn ret = RENDER_OK;
	for (int i = 0; i < 9; ++i) {
		if (m_grids[i]) {
			ret |= DrawNode::Draw(m_grids[i].get(), rp);
		}
	}
	return ret;
}

void Scale9::SetSize(float width, float height)
{
	if (m_width == width && m_height == height) {
		return;
	}

	m_width = width;
	m_height = height;

	switch (m_type)
	{
	case S9_9GRID:
		{
			float w0 = GetLeftWidth(),
				  w2 = GetRightWidth(),
				  w1 = width - w0 - w2;
			float h0 = GetDownHeight(),
				  h2 = GetTopHeight(),
				  h1 = height - h0 - h2;

			ResizeSprite(S9_DOWN_LEFT, sm::vec2(-w0*0.5f-w1*0.5f, -h0*0.5f-h1*0.5f), w0, h0, true, true);
			ResizeSprite(S9_DOWN_CENTER, sm::vec2(0.0f, -h0*0.5f-h1*0.5f), w1, h0, false, true);
			ResizeSprite(S9_DOWN_RIGHT, sm::vec2(w1*0.5f+w2*0.5f, -h0*0.5f-h1*0.5f), w2, h0, true, true);

			ResizeSprite(S9_MID_LEFT, sm::vec2(-w0*0.5f-w1*0.5f, 0.0f), w0, h1, true, false);
			ResizeSprite(S9_MID_CENTER, sm::vec2(0.0f, 0.0f), w1, h1, false, false);
			ResizeSprite(S9_MID_RIGHT, sm::vec2(w1*0.5f+w2*0.5f, 0.0f), w2, h1, true, false);

			ResizeSprite(S9_TOP_LEFT, sm::vec2(-w0*0.5f-w1*0.5f, h1*0.5f+h2*0.5f), w0, h2, true, true);
			ResizeSprite(S9_TOP_CENTER, sm::vec2(0.0f, h1*0.5f+h2*0.5f), w1, h2, false, true);
			ResizeSprite(S9_TOP_RIGHT, sm::vec2(w1*0.5f+w2*0.5f, h1*0.5f+h2*0.5f), w2, h2, true, true);
		}
		break;
	case S9_9GRID_HOLLOW:
		{
			float w0 = GetLeftWidth(),
				  w2 = GetRightWidth(),
				  w1 = width - w0 - w2;
			float h0 = GetDownHeight(),
				  h2 = GetTopHeight(),
				  h1 = height - h0 - h2;

			ResizeSprite(S9_DOWN_LEFT, sm::vec2(-w0*0.5f-w1*0.5f, -h0*0.5f-h1*0.5f), w0, h0, true, true);
			ResizeSprite(S9_DOWN_CENTER, sm::vec2(0.0f, -h0*0.5f-h1*0.5f), w1, h0, false, true);
			ResizeSprite(S9_DOWN_RIGHT, sm::vec2(w1*0.5f+w2*0.5f, -h0*0.5f-h1*0.5f), w2, h0, true, true);

			ResizeSprite(S9_MID_LEFT, sm::vec2(-w0*0.5f-w1*0.5f, 0.0f), w0, h1, true, false);
			ResizeSprite(S9_MID_RIGHT, sm::vec2(w1*0.5f+w2*0.5f, 0.0f), w2, h1, true, false);

			ResizeSprite(S9_TOP_LEFT, sm::vec2(-w0*0.5f-w1*0.5f, h1*0.5f+h2*0.5f), w0, h2, true, true);
			ResizeSprite(S9_TOP_CENTER, sm::vec2(0.0f, h1*0.5f+h2*0.5f), w1, h2, false, true);
			ResizeSprite(S9_TOP_RIGHT, sm::vec2(w1*0.5f+w2*0.5f, h1*0.5f+h2*0.5f), w2, h2, true, true);
		}
		break;
	case S9_6GRID_UPPER:
		{
			float w0 = GetLeftWidth(),
				  w2 = GetRightWidth(),
				  w1 = width - w0 - w2;
			float h2 = GetTopHeight(),
				  h1 = height - h2;

			ResizeSprite(S9_MID_LEFT, sm::vec2(-w0*0.5f-w1*0.5f, 0.0f), w0, h1, true, false);
			ResizeSprite(S9_MID_CENTER, sm::vec2(0.0f, 0.0f), w1, h1, false, false);
			ResizeSprite(S9_MID_RIGHT, sm::vec2(w1*0.5f+w2*0.5f, 0.0f), w2, h1, true, false);

			ResizeSprite(S9_TOP_LEFT, sm::vec2(-w0*0.5f-w1*0.5f, h1*0.5f+h2*0.5f), w0, h2, true, true);
			ResizeSprite(S9_TOP_CENTER, sm::vec2(0.0f, h1*0.5f+h2*0.5f), w1, h2, false, true);
			ResizeSprite(S9_TOP_RIGHT, sm::vec2(w1*0.5f+w2*0.5f, h1*0.5f+h2*0.5f), w2, h2, true, true);
		}
		break;
	case S9_3GRID_HORI:
		{
			float w0 = GetLeftWidth(),
				  w2 = GetRightWidth(),
				  w1 = width - w0 - w2; 

			ResizeSprite(S9_MID_LEFT, sm::vec2(-w0*0.5f-w1*0.5f, 0.0f), w0, height, true, true);
			ResizeSprite(S9_MID_CENTER, sm::vec2(0.0f, 0.0f), w1, height, false, true);
			ResizeSprite(S9_MID_RIGHT, sm::vec2(w1*0.5f+w2*0.5f, 0.0f), w2, height, true, true);
		}
		break;
	case S9_3GRID_VERT:
		{
			float h0 = GetDownHeight(),
				  h2 = GetTopHeight(),
				  h1 = height - h0 - h2;

			ResizeSprite(S9_DOWN_CENTER, sm::vec2(0.0f, -h0*0.5f-h1*0.5f), width, h0, true, true);
			ResizeSprite(S9_MID_CENTER, sm::vec2(0.0f, 0.0f), width, h1, true, false);
			ResizeSprite(S9_TOP_CENTER, sm::vec2(0.0f, h1*0.5f+h2*0.5f), width, h2, true, true);
		}
		break;
	default:
		break;
	}
}

void Scale9::Build(SCALE9_TYPE type, int w, int h, SprPtr grids[9],
				   int sz_left, int sz_right, int sz_top, int sz_down)
{
	m_type = type;
	m_width = m_height = 0;
	for (int i = 0; i < 9; ++i) 
	{
		const SprPtr& src = grids[i];
		if (src) {
			m_grids[i] = src->Clone();
		} else {
			m_grids[i] = nullptr;
		}
	}
	m_sz_left  = sz_left;
	m_sz_right = sz_right;
	m_sz_top   = sz_top;
	m_sz_down  = sz_down;
	SetSize(static_cast<float>(w), static_cast<float>(h));
}

void Scale9::GetGrids(CU_VEC<SprPtr>& grids) const
{
	for (int i = 0; i < 9; ++i) {
		if (m_grids[i]) {
			grids.push_back(m_grids[i]);
		}
	}
}

SCALE9_TYPE Scale9::CheckType(SprPtr grids[9])
{
	SCALE9_TYPE type = S9_NULL;
	do {
		// S9_9GRID
		type = S9_9GRID;
		for (int i = 0; i < 9; ++i) {
			if (!grids[i]) {
				type = S9_NULL;
				break;
			}
		}
		if (type != S9_NULL) break;

		// S9_9GRID_HOLLOW
		type = S9_9GRID_HOLLOW;
		for (int i = 0; i < 9; ++i) {
			if (i == 4) {
				continue;
			}
			if (!grids[i]) {
				type = S9_NULL;
				break;
			}
		}
		if (type != S9_NULL) break;

		// S9_6GRID_UPPER
		type = S9_6GRID_UPPER;
		for (int i = 3; i < 9; ++i) {
			if (!grids[i]) {
				type = S9_NULL;
				break;
			}
		}
		if (type != S9_NULL) break;

		// S9_3GRID_HORI
		if (grids[3] && grids[4] && grids[5]) {
			type = S9_3GRID_HORI;			
		}

		// S9_3GRID_VERT
		if (grids[1] && grids[4] && grids[7]) {
			type = S9_3GRID_VERT;
		}
	} while (false);
	return type;
}

void Scale9::ResizeSprite(SCALE9_IDX idx, const sm::vec2& center, float dst_w, float dst_h,
						  bool no_scale_w, bool no_scale_h)
{
	if (idx < 0 || idx >= 9 || !m_grids[idx]) {
		return;
	}

	auto& spr = m_grids[idx];
	auto img_sym = S2_VI_PTR_DOWN_CAST<ImageSymbol>(spr->GetSymbol());

	if (dst_w < 0) { dst_w = 1; }
	if (dst_h < 0) { dst_h = 1; }

	spr->SetPosition(center);

	float src_w, src_h;
	sm::vec2 sz = img_sym->GetNoTrimedSize();
	if (sz.x == 0 || sz.y == 0) {
		return;
	}
	src_w = sz.x;
	src_h = sz.y;

	const float times = spr->GetAngle() / SM_PI;
	bool rotate = fabs(times - (int)times) > 0.1f;
	if (m_sz_left != 0 && m_sz_right != 0 && m_sz_top != 0 && m_sz_down != 0)
	{
		if (rotate)
		{
			switch (idx)
			{
			case S9_DOWN_LEFT:
				src_h = static_cast<float>(m_sz_left);
				src_w = static_cast<float>(m_sz_down);
				break;
			case S9_DOWN_CENTER:
				src_w = static_cast<float>(m_sz_down);
				break;
			case S9_DOWN_RIGHT:
				src_h = static_cast<float>(m_sz_right);
				src_w = static_cast<float>(m_sz_down);
				break;
			case S9_MID_LEFT:
				src_h = static_cast<float>(m_sz_left);
				break;
			case S9_MID_RIGHT:
				src_h = static_cast<float>(m_sz_right);
				break;
			case S9_TOP_LEFT:
				src_h = static_cast<float>(m_sz_left);
				src_w = static_cast<float>(m_sz_top);
				break;
			case S9_TOP_CENTER:
				src_w = static_cast<float>(m_sz_top);
				break;
			case S9_TOP_RIGHT:
				src_h = static_cast<float>(m_sz_right);
				src_w = static_cast<float>(m_sz_top);
				break;
			default:
				break;
			}
		}
		else
		{
			switch (idx)
			{
			case S9_DOWN_LEFT:
				src_w = static_cast<float>(m_sz_left);
				src_h = static_cast<float>(m_sz_down);
				break;
			case S9_DOWN_CENTER:
				src_h = static_cast<float>(m_sz_down);
				break;
			case S9_DOWN_RIGHT:
				src_w = static_cast<float>(m_sz_right);
				src_h = static_cast<float>(m_sz_down);
				break;
			case S9_MID_LEFT:
				src_w = static_cast<float>(m_sz_left);
				break;
			case S9_MID_RIGHT:
				src_w = static_cast<float>(m_sz_right);
				break;
			case S9_TOP_LEFT:
				src_w = static_cast<float>(m_sz_left);
				src_h = static_cast<float>(m_sz_top);
				break;
			case S9_TOP_CENTER:
				src_h = static_cast<float>(m_sz_top);
				break;
			case S9_TOP_RIGHT:
				src_w = static_cast<float>(m_sz_right);
				src_h = static_cast<float>(m_sz_top);
				break;
			default:
				break;
			}
		}
	}

	const sm::vec2& old_scale = spr->GetScale();
	sm::vec2 new_scale;	
	if (rotate) {
		new_scale.x = no_scale_h ? 1 : dst_h / src_w;
		new_scale.y = no_scale_w ? 1 : dst_w / src_h;
	} else {
		new_scale.x = no_scale_w ? 1 : dst_w / src_w;
		new_scale.y = no_scale_h ? 1 : dst_h / src_h;
	}
	if (old_scale.x < 0) {
		new_scale.x = -new_scale.x;
	}
	if (old_scale.y < 0) {
		new_scale.y = -new_scale.y;
	}
	spr->SetScale(new_scale);

	spr->Translate(sm::rotate_vector(spr->GetOffset(), spr->GetAngle()) - spr->GetOffset());
}

sm::vec2 Scale9::GetChildSize(SCALE9_IDX idx) const
{
	if (idx < 0 || idx >= 9 || !m_grids[idx]) {
		return sm::vec2(0, 0);
	}

	auto& spr = m_grids[idx];
	auto& sym = spr->GetSymbol();
	if (sym->Type() == SYM_IMAGE) {
		return S2_VI_PTR_DOWN_CAST<const ImageSymbol>(sym)->GetNoTrimedSize();
	} else {
		return sym->GetBounding(spr.get()).Size();
	}
}

float Scale9::GetLeftWidth() const
{
	return m_sz_left == 0 ? 
		std::max(std::max(GetChildSize(S9_DOWN_LEFT).x, GetChildSize(S9_MID_LEFT).x), GetChildSize(S9_TOP_LEFT).x) : m_sz_left;
}

float Scale9::GetRightWidth() const
{
	return m_sz_right == 0 ? 
		std::max(std::max(GetChildSize(S9_DOWN_RIGHT).x, GetChildSize(S9_MID_RIGHT).x), GetChildSize(S9_TOP_RIGHT).x) : m_sz_right;
}

float Scale9::GetDownHeight() const
{
	return m_sz_down == 0 ?
		std::max(std::max(GetChildSize(S9_DOWN_LEFT).y, GetChildSize(S9_DOWN_CENTER).y), GetChildSize(S9_DOWN_RIGHT).y) : m_sz_down;
}

float Scale9::GetTopHeight() const
{
	return m_sz_top == 0 ?
		std::max(std::max(GetChildSize(S9_TOP_LEFT).y, GetChildSize(S9_TOP_CENTER).y), GetChildSize(S9_TOP_RIGHT).y) : m_sz_top;
}

}