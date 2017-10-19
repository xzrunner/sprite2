#include "StatTopNodes.h"

#include <glp_loop.h>
#include <stdio.h>
#include <assert.h>

namespace s2
{

CU_SINGLETON_DEFINITION(StatTopNodes);

static const int TOP_NODE_NUM = 20;

StatTopNodes::StatTopNodes()
	: m_min_time(0)
{
}

void StatTopNodes::Print(CU_STR& str) const
{
	static char buf[512];
	CU_LIST<DrawNode>::const_iterator itr = m_nodes.begin();
	for ( ; itr != m_nodes.end(); ++itr) {
		const DrawNode& n = *itr;
		sprintf(buf, "cost: %d, id: %u, pid: %u, level %d\n", n.cost, n.id, n.parent_id, n.level);
		str += buf;
	}
}

void StatTopNodes::Reset()
{
	m_nodes.clear();
	m_min_time = 0;
}

void StatTopNodes::Insert(const DrawNode& node)
{
	if (node.cost == 0) {
		return;
	}

	if (m_nodes.empty()) {
		m_nodes.push_back(node);
		m_min_time = node.cost;
		return;
	}

	if (node.cost <= m_min_time) {
		if (m_nodes.size() < TOP_NODE_NUM) {
			m_nodes.push_back(node);
			m_min_time = node.cost;
		}
		return;
	}

	CU_LIST<DrawNode>::iterator itr = m_nodes.begin();
	for ( ; itr != m_nodes.end(); ++itr) 
	{
		const DrawNode& n = *itr;
		if (node.cost > n.cost) 
		{
			m_nodes.insert(itr, node);
			if (m_nodes.size() > TOP_NODE_NUM) {
				m_nodes.pop_back();
				int num = m_nodes.size();
				m_min_time = m_nodes.back().cost;
			}
			return;
		}
	}

	assert(m_nodes.size() < TOP_NODE_NUM);

	m_nodes.push_back(node);
	m_min_time = node.cost;
}

/************************************************************************/
/* class StatTopNodes::Checkpoint                                       */
/************************************************************************/

StatTopNodes::Checkpoint::Checkpoint(uint32_t id, uint32_t parent_id, int level)
	: m_id(id)
	, m_parent_id(parent_id)
	, m_level(level)
{
	m_begin = glp_get_time();
}

StatTopNodes::Checkpoint::~Checkpoint()
{
	uint32_t cost = glp_get_time() - m_begin;
	StatTopNodes::Instance()->Insert(DrawNode(m_id, m_parent_id, m_level, cost));
}

}