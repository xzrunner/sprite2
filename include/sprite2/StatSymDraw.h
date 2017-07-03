#ifndef _SPRITE2_STAT_SYM_DRAW_H_
#define _SPRITE2_STAT_SYM_DRAW_H_

#include "StatSymType.h"

#include <CU_Singleton.h>

#include <string>

#include <stdint.h>

namespace s2
{

class StatSymDraw
{
public:
	void Print(std::string& str) const;
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

		void Print(std::string& str, const std::string& tag) const;

		void Reset();

	private:
		int      m_draw_count;
		uint32_t m_draw_cost;

		friend class StatSymDraw;

	}; // Item

private:
	Item m_items[STAT_SYM_MAX];
	
	SINGLETON_DECLARATION(StatSymDraw);

}; // StatSymDraw

}

#endif // _SPRITE2_STAT_SYM_DRAW_H_