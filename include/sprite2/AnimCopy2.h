#ifndef _SPRITE2_ANIM_COPY2_H_
#define _SPRITE2_ANIM_COPY2_H_

#include <SM_Vector.h>

#include <vector>

#include <stdint.h>

namespace s2
{

class Sprite;
class AnimSymbol;

class AnimCopy2
{
public:
	AnimCopy2();
	~AnimCopy2();

	void LoadFromSym(const AnimSymbol& sym);
	
private:
	struct LerpData
	{
		sm::vec2 pos, dpos;
	};

	struct Node
	{
		int16_t curr;
		int16_t next, prev;

		int16_t lerp_data;

		Node() : curr(-1), next(-1), prev(-1), lerp_data(-1) {}
	};

	struct Frame
	{
		int time;
		std::vector<Node> nodes;
	};

	struct Layer
	{
		std::vector<Frame> frames;
	};

private:
	std::vector<Layer> m_layers;

	std::vector<const Sprite*> m_slots;

	std::vector<LerpData> m_lerp_data;

	int m_max_frame_idx;
	int m_max_node_num;

	friend class AnimCurr3;

}; // AnimCopy2

}

#endif // _SPRITE2_ANIM_COPY2_H_