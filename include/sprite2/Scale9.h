#ifndef _SPRITE2_SCALE9_H_
#define _SPRITE2_SCALE9_H_

#include "RenderReturn.h"
#include "s2_typedef.h"

#include <SM_Vector.h>
#include <cu/cu_stl.h>

#include <memory>

namespace cooking { class DisplayList; }

namespace s2
{

class Sprite;
class RenderParams;

enum SCALE9_TYPE
{
	S9_NULL = 0,
	S9_9GRID,
	S9_3GRID_HORI,
	S9_3GRID_VERT,
	S9_6GRID_UPPER,
	S9_9GRID_HOLLOW
};

enum SCALE9_IDX
{
	S9_DOWN_LEFT	= 0,
	S9_DOWN_CENTER	= 1,
	S9_DOWN_RIGHT	= 2,

	S9_MID_LEFT		= 3,
	S9_MID_CENTER	= 4,
	S9_MID_RIGHT	= 5,

	S9_TOP_LEFT		= 6,
	S9_TOP_CENTER	= 7,
	S9_TOP_RIGHT	= 8,
};

class Scale9
{
public:
	Scale9();
	Scale9(const Scale9& s9);
	Scale9& operator = (const Scale9& s9);

	RenderReturn Draw(const RenderParams& rp) const;

	void SetSize(float width, float height);
	sm::vec2 GetSize() const { return sm::vec2(m_width, m_height); }

	void Build(SCALE9_TYPE type, int w, int h, SprPtr grids[9],
		int sz_left, int sz_right, int sz_top, int sz_down);

	void GetGrids(CU_VEC<SprPtr>& grids) const;
	const SprPtr& GetGrid(SCALE9_IDX idx) const { return m_grids[idx]; }

	SCALE9_TYPE GetType() const { return m_type; }

	static SCALE9_TYPE CheckType(SprPtr grids[9]);

private:
	void ResizeSprite(SCALE9_IDX idx, const sm::vec2& center, 
		float dst_w, float dst_h, bool no_scale_w, bool no_scale_h);

	sm::vec2 GetChildSize(SCALE9_IDX idx) const;
	
	float GetLeftWidth() const;
	float GetRightWidth() const;
	float GetDownHeight() const;
	float GetTopHeight() const;

private:
	SCALE9_TYPE m_type;

	float m_width, m_height;

	// 6 7 8
	// 3 4 5
	// 0 1 2
	SprPtr m_grids[9];

	int m_sz_left, m_sz_right, m_sz_top, m_sz_down;

}; // Scale9

}

#endif // _SPRITE2_SCALE9_H_