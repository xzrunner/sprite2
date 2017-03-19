#ifndef _SPRITE2_ANIM_COPY_H_
#define _SPRITE2_ANIM_COPY_H_

#include "Color.h"
#include "SprSRT.h"

#include <SM_Vector.h>

#include <vector>

#include <stdint.h>

namespace s2
{

class Sprite;
class AnimSymbol;

class AnimCopy
{
public:
	AnimCopy();
	~AnimCopy();

	void LoadFromSym(const AnimSymbol& sym);
	
private:
	void SetCountNum(const AnimSymbol& sym);
	void FillingLayers(const AnimSymbol& sym);
	void ConnectActors(const AnimSymbol& sym);
	void LoadLerpData(const AnimSymbol& sym);
	void CreateSprSlots(const AnimSymbol& sym);

	static void CalcDeltaColor(const Color& begin, const Color& end, int time, float* ret);

private:
	struct Lerp
	{
		SprSRT srt, dsrt;

		Color col_mul, col_add;
		float dcol_mul[4], dcol_add[4];
	};

	struct Actor
	{
		int16_t next, prev;
		int16_t slot;
		int16_t lerp;

		const Sprite* spr;

		Actor();
		~Actor();
	};

	struct Frame
	{
		int time;
		std::vector<Actor> actors;
	};

	struct Layer
	{
		std::vector<Frame> frames;
	};

private:
	std::vector<Layer> m_layers;

	std::vector<const Sprite*> m_slots;

	std::vector<Lerp> m_lerps;

	std::vector<const Sprite*> m_sprites;	

	int m_max_frame_idx;
	int m_max_actor_num;

	friend class AnimCurr;

}; // AnimCopy

}

#endif // _SPRITE2_ANIM_COPY_H_