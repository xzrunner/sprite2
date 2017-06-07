#ifndef _SPRITE2_STAT_SYMBOL_H_
#define _SPRITE2_STAT_SYMBOL_H_

#include <CU_Singleton.h>

#include <string>

#include <stdint.h>

namespace s2
{

class StatSymbol
{
public:
	enum Type
	{
		SYM_IMAGE = 0,
		SYM_SCALE9,
		SYM_ICON,
		SYM_TEXTURE,
		SYM_TEXTBOX,
		SYM_COMPLEX,
		SYM_ANIMATION,
		SYM_ANIM2,
		SYM_PARTICLE3D,
		SYM_PARTICLE2D,
		SYM_SHAPE,
		SYM_MESH,
		SYM_MASK,
		SYM_TRAIL,
		SYM_SKELETON,

		SYM_MAX,
	};

public:
	void Print(std::string& str) const;
	void Reset();

	void AddDrawCount(Type type);
	void AddDrawCost(Type type, uint32_t cost);

public:
	class DrawCostCP
	{
	public:
		DrawCostCP(Type type);
		~DrawCostCP();

	private:
		Type m_type;
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

		friend class StatSymbol;

	}; // Item

private:
	Item m_items[SYM_MAX];
	
	SINGLETON_DECLARATION(StatSymbol);

}; // StatSymbol

}

#endif // _SPRITE2_STAT_SYMBOL_H_