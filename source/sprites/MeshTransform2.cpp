#include "MeshTransform2.h"
#include "MeshTriangle.h"
#include "Mesh.h"

namespace s2
{

void MeshTransform2::LoadFromMesh(const Mesh* mesh)
{
	m_trans.clear();

	const std::vector<MeshTriangle*>& tris = mesh->GetTriangles();
	int idx = 0;
	for (int i = 0, n = tris.size(); i < n; ++i) {
		for (int j = 0; j < 3; ++j) {
			MeshNode* node = tris[i]->nodes[j];
			if (node->xy != node->ori_xy) {
				m_trans.push_back(std::make_pair(idx, node->xy - node->ori_xy));
			}
			++idx;
		}
	}
}

void MeshTransform2::StoreToMesh(const Mesh* mesh) const
{
	const std::vector<MeshTriangle*>& tris = mesh->GetTriangles();

	// clear
	for (int i = 0, n = tris.size(); i < n; ++i) {
		for (int j = 0; j < 3; ++j) {
			MeshNode* node = tris[i]->nodes[j];
			node->xy = node->ori_xy;
		}
	}

	// set
	std::vector<std::pair<int, sm::vec2> >::const_iterator itr = m_trans.begin();
	for ( ; itr != m_trans.end(); ++itr) {
		int tri = itr->first / 3;
		int node = itr->first - tri * 3;
		tris[tri]->nodes[node]->xy += itr->second;
	}
}

void MeshTransform2::Lerp(const MeshTransform2& begin, const MeshTransform2& end, float process)
{
	int b_ptr = 0, e_ptr = 0;
	int bn = begin.m_trans.size(), en = end.m_trans.size();
	while (b_ptr < bn && e_ptr < en)
	{
		int b_idx = begin.m_trans[b_ptr].first;
		int e_idx = end.m_trans[e_ptr].first;
		const sm::vec2& b_pos = begin.m_trans[b_ptr].second;
		const sm::vec2& e_pos = end.m_trans[e_ptr].second;
		if (b_idx == e_idx) 
		{
			sm::vec2 pos = b_pos + (e_pos - b_pos) * process;
			m_trans.push_back(std::make_pair(b_idx, pos));
			++b_idx;
			++e_idx;
		} 
		else if (b_idx < e_idx) 
		{
			m_trans.push_back(std::make_pair(b_idx, b_pos * (1 - process)));
			++b_idx;
		} 
		else 
		{
			m_trans.push_back(std::make_pair(e_idx, e_pos * process));
			++e_idx;
		}
	}

	if (b_ptr != bn) 
	{
		for (int i = b_ptr; i < bn; ++i) {
			m_trans.push_back(std::make_pair(begin.m_trans[i].first, 
				begin.m_trans[i].second * (1 - process)));
		}
	} 
	else if (e_ptr != en) 
	{
		for (int i = e_ptr; i < en; ++i) {
			m_trans.push_back(std::make_pair(end.m_trans[i].first, 
				end.m_trans[i].second * process));
		}
	}
}

}