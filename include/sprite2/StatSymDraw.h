#ifndef _SPRITE2_STAT_SYM_DRAW_H_
#define _SPRITE2_STAT_SYM_DRAW_H_

#include "sprite2/StatSymType.h"

#include <cu/cu_macro.h>
#include <cu/cu_stl.h>

#include <stdint.h>

namespace s2
{

class StatSymDraw
{
public:
	void Print(CU_STR& str) const;
	void Reset();

	void AddDrawCount(StatSymType type);
	void AddDrawCost(StatSymType type, uint32_t cost);

public:
	class DrawCostCP
	{
	public:
		DrawCostCP(StatSymType type);
		~DrawCostCP();

	private:
		StatSymType m_type;
		uint32_t m_begin;

	}; // DrawCostCP

private:
	class Item
	{
	public:
		Item();

		void Print(CU_STR& str, const CU_STR& tag) const;

		void Reset();

	private:
		int      m_draw_count;
		uint32_t m_draw_cost;

		friend class StatSymDraw;

	}; // Item

private:
	Item m_items[STAT_SYM_MAX];
	
	CU_SINGLETON_DECLARATION(StatSymDraw);

}; // StatSymDraw

}

#endif // _SPRITE2_STAT_SYM_DRAW_H_