#ifndef _SPRITE2_ANIM_COPY_H_
#define _SPRITE2_ANIM_COPY_H_

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
	struct Node
	{
		int16_t curr;
		int16_t next;

		Node() : curr(-1), next(-1) {}
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

	int m_max_frame_idx;
	int m_max_node_num;

	friend class AnimCurr2;

}; // AnimCopy

}

#endif // _SPRITE2_ANIM_COPY_H_