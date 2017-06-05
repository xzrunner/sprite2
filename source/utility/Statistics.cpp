#include "sprite2/Statistics.h"

#include <glp_loop.h>

#include <string.h>
#include <assert.h>

namespace s2
{

SINGLETON_DEFINITION(Statistics);

Statistics::Statistics() 
	: m_overdraw_area(0) 
{}

void Statistics::Reset()
{
	m_overdraw_area = 0;
	m_pp_count.Reset();
	m_dc_count.Reset();

	m_top_nodes.clear();
	m_top_min_time = 0;
}

void Statistics::TopNodeInsert(const DrawNode& node)
{
	if (node.cost == 0) {
		return;
	}

	if (m_top_nodes.empty()) {
		m_top_nodes.push_back(node);
		m_top_min_time = node.cost;
		return;
	}

	if (node.cost <= m_top_min_time) {
		if (m_top_nodes.size() < TOP_NODE_NUM) {
			m_top_nodes.push_back(node);
			m_top_min_time = node.cost;
		}
		return;
	}

	std::list<DrawNode>::iterator itr = m_top_nodes.begin();
	for ( ; itr != m_top_nodes.end(); ++itr) 
	{
		const DrawNode& n = *itr;
		if (node.cost > n.cost) 
		{
			m_top_nodes.insert(itr, node);
			if (m_top_nodes.size() > TOP_NODE_NUM) {
				m_top_nodes.pop_back();
				int num = m_top_nodes.size();
				m_top_min_time = m_top_nodes.back().cost;
			}
			return;
		}
	}

	assert(m_top_nodes.size() < TOP_NODE_NUM);

	m_top_nodes.push_back(node);
	m_top_min_time = node.cost;
}

/************************************************************************/
/* class Statistics::PingPongCount                                      */
/************************************************************************/

Statistics::PingPongCount::PingPongCount() 
{
	Reset();
}

void Statistics::PingPongCount::Reset() 
{
	memset(this, 0, sizeof(*this));
}

/************************************************************************/
/* class Statistics::DrawCallCount                                      */
/************************************************************************/

Statistics::DrawCallCount::DrawCallCount() 
{
	Reset();
}

void Statistics::DrawCallCount::Reset() 
{
	memset(this, 0, sizeof(*this));
}

/************************************************************************/
/* class Statistics::Checkpoint                                         */
/************************************************************************/

Statistics::Checkpoint::Checkpoint(uint32_t id, uint32_t parent_id, int level)
	: m_id(id)
	, m_parent_id(parent_id)
	, m_level(level)
{
	m_begin = glp_get_time();
}

Statistics::Checkpoint::~Checkpoint()
{
	uint32_t cost = glp_get_time() - m_begin;
	Statistics* stat = Statistics::Instance();
	stat->TopNodeInsert(DrawNode(m_id, m_parent_id, m_level, cost));
}

}
