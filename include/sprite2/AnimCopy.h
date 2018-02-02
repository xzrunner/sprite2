#ifndef _SPRITE2_ANIM_COPY_H_
#define _SPRITE2_ANIM_COPY_H_

#include "sprite2/AnimLerp.h"

#include <SM_Vector.h>
#include <cu/cu_stl.h>
#include <painting2/Color.h>
#include <painting2/SRT.h>

#include <stdint.h>

namespace s2
{

class Sprite;
class Actor;
class AnimSymbol;

class AnimCopy
{
public:
	AnimCopy();

	void LoadFromSym(const AnimSymbol& sym);
	
private:
	void SetCountNum(const AnimSymbol& sym);
	void FillingLayers(const AnimSymbol& sym);
	void ConnectItems(const AnimSymbol& sym);
	void LoadLerpData(const AnimSymbol& sym);
	void CreateSprSlots(const AnimSymbol& sym);

	static void CalcDeltaColor(const pt2::Color& begin, const pt2::Color& end, int time, float* ret);

private:
	struct Lerp
	{
		pt2::SRT srt, dsrt;

		pt2::Color col_mul, col_add;
		float dcol_mul[4], dcol_add[4];
	};

	struct Item
	{
		int16_t next, prev;
		int16_t slot;
		int16_t lerp;

		SprConstPtr spr = nullptr;

		Item();
	};

	struct Frame
	{
		int time;
		CU_VEC<Item> items;

		CU_VEC<std::pair<AnimLerp::SprData, std::unique_ptr<ILerp>>> lerps;
	};

	struct Layer
	{
		CU_VEC<Frame> frames;
	};

private:
	CU_VEC<Layer> m_layers;

	CU_VEC<SprConstPtr> m_slots;

	CU_VEC<Lerp> m_lerps;

	CU_VEC<SprConstPtr> m_sprites;

	int m_max_frame_idx;
	int m_max_item_num;

	friend class AnimCurr;

}; // AnimCopy

}

#endif // _SPRITE2_ANIM_COPY_H_