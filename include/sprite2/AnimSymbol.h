#ifndef _SPRITE2_ANIM_SYMBOL_H_
#define _SPRITE2_ANIM_SYMBOL_H_

#include "s2_config.h"

#include "AnimLerp.h"
#include "S2_Symbol.h"
#ifdef S2_ANIM_CURR_OLD
#include "AnimCurr.h"
#else
#include "AnimCurr2.h"
#include "AnimCopy.h"
#endif // S2_ANIM_CURR_OLD

#include <vector>

namespace s2
{

class AnimSymbol : public VIRTUAL_INHERITANCE Symbol
{
public:
	class Frame
	{
	public:
		int index;
		std::vector<Sprite*> sprs;

		bool tween;
		std::vector<std::pair<AnimLerp::SprData, ILerp*> > lerps;

	public:
		Frame() : index(0), tween(false) {}

	}; // Frame

	class Layer
	{
	public:
		std::string name;
		std::vector<Frame*> frames;

	public:
		Frame* GetCurrFrame(int index) const;
		Frame* GetNextFrame(int index) const;

	}; // Layer

public:
	AnimSymbol();
	AnimSymbol(uint32_t id);
	virtual ~AnimSymbol();

	/**
	 *  @interface
	 *    Symbol
	 */
	virtual int Type() const;
	virtual void Traverse(const SymVisitor& visitor);
	virtual void Draw(const RenderParams& params, const Sprite* spr = NULL) const;
	virtual bool Update(const RenderParams& params, float time);
	virtual sm::rect GetBounding(const Sprite* spr = NULL) const;

	const std::vector<Layer*>& GetLayers() const { return m_layers; }
	int GetMaxFrameIdx() const;

	void CreateFrameSprites(int frame, std::vector<Sprite*>& sprs) const;

	int GetFPS() const { return m_fps; }
	void SetFPS(int fps) { m_fps = fps; }

#ifndef S2_ANIM_CURR_OLD
	const AnimCopy& GetCopy() const { return m_copy; }
	void LoadCopy();
#endif // S2_ANIM_CURR_OLD

	/************************************************************************/
	/* api for dynamic change                                               */
	/************************************************************************/
	void AddLayer(Layer* layer);
	bool Clear();

private:
	void Draw(const RenderParams& params, int idx = 1);

protected:
	std::vector<Layer*> m_layers;

	int m_fps;

#ifdef S2_ANIM_CURR_OLD
	AnimCurr m_curr;
#else
	AnimCopy  m_copy;
	AnimCurr2 m_curr;
#endif // S2_ANIM_CURR_OLD

	mutable sm::rect m_size;

}; // AnimSymbol

}

#endif // _SPRITE2_ANIM_SYMBOL_H_