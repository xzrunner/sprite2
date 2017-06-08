#ifndef _SPRITE2_STAT_TOP_NODES_H_
#define _SPRITE2_STAT_TOP_NODES_H_

#include <CU_Singleton.h>

#include <list>
#include <string>

#include <stdint.h>

namespace s2
{

class StatTopNodes
{	
public:
	struct DrawNode
	{
		uint32_t id;
		uint32_t parent_id;
		int      level;
		uint32_t cost;

		DrawNode(uint32_t id, uint32_t parent_id, int level, uint32_t cost) 
			: id(id), parent_id(parent_id), level(level), cost(cost) {}
	};

public:
	void Print(std::string& str) const;
	void Reset();

	void Insert(const DrawNode& node);

public:
	class Checkpoint
	{
	public:
		Checkpoint(uint32_t id, uint32_t parent_id, int level);
		~Checkpoint();

	private:
		uint32_t m_begin;

		uint32_t m_id;
		uint32_t m_parent_id;
		int      m_level;

	}; // Checkpoint

private:
	static const int TOP_NODE_NUM = 10;

private:
	std::list<DrawNode> m_nodes;
	uint32_t m_min_time;

	SINGLETON_DECLARATION(StatTopNodes);

}; // StatTopNodes

}

#endif // _SPRITE2_STAT_TOP_NODES_H_