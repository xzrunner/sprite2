#ifndef _SPRITE2_STATISTICS_H_
#define _SPRITE2_STATISTICS_H_

#include <CU_Singleton.h>

#include <list>

#include <stdint.h>

namespace s2
{

class Statistics
{
public:
	class PingPongCount
	{
	public:
		int mesh;
		int mask;
		int blend;
		int gaussian_blur;
		int outer_glow;
		int down_sample;
		int rt_outside;

	public:
		PingPongCount();

		void Reset();

	}; // PingPongCount

	class DrawCallCount
	{
	public:
		int scissor;

	public:
		DrawCallCount();

		void Reset();

	}; // DrawCallCount

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

		friend class Statistics;

	}; // Checkpoint

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
	void AddOverdrawArea(float area) { m_overdraw_area += area; }

	float GetOverdrawArea() const { return m_overdraw_area; }

	// ping pong
	void AddMesh() { ++m_pp_count.mesh; }
	void AddMask() { ++m_pp_count.mask; }
	void AddBlend() { ++m_pp_count.blend; }
	void AddGaussianBlur() { ++m_pp_count.gaussian_blur; }
	void AddOuterGlow() { ++m_pp_count.outer_glow; }
	void AddDownSample() { ++m_pp_count.down_sample; }
	void AddRTOutside() { ++m_pp_count.rt_outside; }
	const PingPongCount& GetPingPongCount() const { return m_pp_count; }

	// draw call
	void AddScissor() { ++m_dc_count.scissor; }
	const DrawCallCount& GetDrawCallCount() const { return m_dc_count; }

	const std::list<DrawNode>& GetTopNodes() const { return m_top_nodes; }

	void Reset();

private:
	static const int TOP_NODE_NUM = 10;

private:
	void TopNodeInsert(const DrawNode& node);

private:
	float m_overdraw_area;

	PingPongCount m_pp_count;
	DrawCallCount m_dc_count;

	// node's draw time
	std::list<DrawNode> m_top_nodes;
	uint32_t m_top_min_time;

	SINGLETON_DECLARATION(Statistics);

}; // Statistics

}

#endif // _SPRITE2_STATISTICS_H_